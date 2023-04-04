// solution to: https://www.youtube.com/watch?v=_-AfhLQfb6w
// g++ main.cpp -std=c++20 -O3 -Wall
#include <bits/stdc++.h>

std::string get_file_contents(const std::string &filename)
{
  std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
  if (in) {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    return(contents.str());
  }
  throw(errno);
}

using small_5w_t = unsigned int;
struct ext_5w_t {
  ext_5w_t(const char *p) {
    for(size_t i = 0; i < 5; ++i)
      w5[i] = p[i];
  }
  char w5[5]; 
};
using w_t = std::pair<std::vector<small_5w_t>, std::vector<ext_5w_t>>;

w_t parse_words(const std::string &data, bool remove_anagrams=true) {
  w_t words;
    words.first.reserve(data.size() / 6);
    words.second.reserve(data.size() / 6);

    small_5w_t current = 0, count = 0;
    for(size_t i = 0; i < data.size(); i++) {
        if(data[i] == '\n') {
            if(count == 5) {
              if((!remove_anagrams) || 
                 (std::find(words.first.begin(), words.first.end(), current) == words.first.end())){
                words.first.push_back(current);
                words.second.emplace_back(&data[i] - 5);
              }
            }
            current = 0;
            count = 0;
        } else {
            const auto letter = 1 << (data[i] - 'a');
            if((current & letter) || (count > 5)) {
                while(i < data.size() && data[i] != '\n') {
                    i++;
                }
                current = 0;
                count = 0;
            } else {
                current = current | letter;
                count++;
            }
        }
    }
    if(current && count == 5) {
      if((!remove_anagrams) || 
         (std::find(words.first.begin(), words.first.end(), current) == words.first.end())){
        words.first.push_back(current);
        words.second.emplace_back(&data.back() - 5);
      }
    }
    return words;
}

void brute_force(const w_t &words) {
  const auto &ws = words.first;
  for(size_t i0 = 0; i0 < ws.size(); ++i0) {
    const auto w0 = ws[i0];
    std::cout << i0 << "/" << ws.size() << "\n";
    for(size_t i1 = i0; i1 < ws.size(); ++i1) {
      const auto w1 = ws[i1];
      if(w0 & w1) continue;
      const auto current1 = w0 | w1;
      for(size_t i2 = i1; i2 < ws.size(); ++i2) {
        const auto w2 = ws[i2];
        if(current1 & w2) continue;
        const auto current2 = current1 | w2;
        for(size_t i3 = i2; i3 < ws.size(); ++i3) {
          const auto w3 = ws[i3];
          if(current2 & w3) continue;
          const auto current3 = current2 | w3;
          for(size_t i4 = i3; i4 < ws.size(); ++i4) {
            const auto w4 = ws[i4];
            if(current3 & w4) continue;
            std::cout << std::string(words.second[i0].w5, 5) << " " 
                      << std::string(words.second[i1].w5, 5) << " " 
                      << std::string(words.second[i2].w5, 5) << " " 
                      << std::string(words.second[i3].w5, 5) << " " 
                      << std::string(words.second[i4].w5, 5) << " " 
                      << "\n";
          }
        }
      }
    }
  }
}

void print_dp_solution(const std::vector<std::vector<bool>> &table,
          const w_t &words,
          std::vector<size_t> &result,
          small_5w_t hash,
          size_t start_from) {
  if (result.size() == 5) {
    for (int i = 0; i < 5; ++i) {
      std::cout << std::string(words.second[result[i]].w5, 5) << ' ';
    }
    std::cout << "\n";
    return;
  }
  for (size_t i = start_from; i < words.second.size(); ++i) {
    // the word words.first[i] is contained in hash and
    // the remaining can be built
    if (
        ((hash & words.first[i]) == words.first[i]) && 
        (result.size() == 4 || table[3 - result.size()][hash ^ words.first[i]])
      ) {
      result.push_back(i);
      print_dp_solution(table, words, result, hash ^ words.first[i], i + 1);
      result.pop_back();
    }
  }
}

void dp_solve(const w_t &words) {
  // std::vector<bool>(1 << 26) is one (or more) word without repetition
  // of any letter
  std::vector<std::vector<bool>> table(5, std::vector<bool>(1 << 26));
  for(const auto &w : words.first) table[0][w] = true;
  for (int lvl = 0; lvl < 4; ++lvl) {
    std::cout << "lvl: " << lvl + 1 << "\n";
    for (small_5w_t hash = 0u; hash < (1u << 26u); ++hash) {
      if (!table[lvl][hash]) continue;
      for (size_t i = 0; i < words.first.size(); ++i) {
        if (!(words.first[i] & hash)) {
          table[lvl + 1][words.first[i] | hash] = true;
        }
      }
    }
  }

  std::cout << "DP table filled" << std::endl;

  std::vector<size_t> solution;
  solution.reserve(5);
  for (int i = 0; i < 26; ++i) {
    const small_5w_t hash = ((1u << 26u) - 1u) ^ (1 << i);
    // std::bitset<32> x(hash); std::cout << x << std::endl;
    if (table[4][hash]) {
      print_dp_solution(table, words, solution, hash, 0);
    }
  }
}

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main() {

  auto t1 = high_resolution_clock::now();
  const auto data = get_file_contents("words_alpha.txt");
  auto t2 = high_resolution_clock::now();
  // you can decide to remove the anagrams of words
  const auto words = parse_words(data, true);
  auto t3 = high_resolution_clock::now();

  std::cout << "reading file: " << duration_cast<milliseconds>(t2 - t1).count() << "ms\n";
  std::cout << "parsing file: " << duration_cast<milliseconds>(t3 - t2).count() << "ms\n";
  //brute_force(words);
  dp_solve(words);

  return 0;
}