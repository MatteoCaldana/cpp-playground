#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

// https://stackoverflow.com/questions/5852261/c-tuple-vs-struct
// https://godbolt.org/z/h3eaEPv8q

class Timer {
public:
  Timer() { reset(); }
  void reset() { start = now(); }

  double getElapsedSeconds() {
    std::chrono::duration<double> seconds = now() - start;
    return seconds.count();
  }

private:
  static std::chrono::time_point<std::chrono::high_resolution_clock> now() {
    return std::chrono::high_resolution_clock::now();
  }

  std::chrono::time_point<std::chrono::high_resolution_clock> start;

};

struct ST {
  int X;
  int Y;
  double Cost;
  std::string Label;

  bool operator==(const ST &rhs) {
    return
      (X == rhs.X) &&
      (Y == rhs.Y) &&
      (Cost == rhs.Cost) 
      && (Label == rhs.Label)
      ;
  }

  bool operator<(const ST &rhs) {
    if(X > rhs.X) { return false; }
    if(Y > rhs.Y) { return false; }
    if(Cost > rhs.Cost) { return false; }
    if(Label >= rhs.Label) { return false; }
    return true;
  }


    // bool operator==(const ST &rhs) {
    //     return std::tie(X,Y,Cost, Label) == std::tie(rhs.X, rhs.Y, rhs.Cost, rhs.Label);
    // }

    // void swap(ST & other)
    // {
    //     std::swap(X, other.X);
    //     std::swap(Y, other.Y);
    //     std::swap(Cost, other.Cost);
    //     std::swap(Label, other.Label);
    // }  

    // bool operator<(const ST &rhs) {
    //     return std::tie(X,Y,Cost, Label) < std::tie(rhs.X, rhs.Y, rhs.Cost, rhs.Label);
    // }
};

using TP = std::tuple<int, int, double , std::string>;

std::pair<std::vector<ST>, std::vector<TP>> generate() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution<int> dist;

  constexpr size_t SZ = 1000000;

  std::pair<std::vector<ST>, std::vector<TP>> p;
  auto& s = p.first;
  auto& d = p.second;
  s.reserve(SZ);
  d.reserve(SZ);

  for(size_t i = 0; i < SZ; i++) {
    s.emplace_back();
    auto& sb = s.back();
    sb.X = dist(mt);
    sb.Y = dist(mt);
    sb.Cost = sb.X * sb.Y;
    sb.Label = std::to_string(sb.Cost);

    d.emplace_back(std::tie(sb.X, sb.Y, sb.Cost, sb.Label));
  }

  return p;
}

int main() {
  Timer timer;

  auto p = generate();
  auto& structs = p.first;
  auto& tuples = p.second;

  timer.reset();
  std::sort(structs.begin(), structs.end());
  double stSecs = timer.getElapsedSeconds();

  timer.reset();
  std::sort(tuples.begin(), tuples.end());
  double tpSecs = timer.getElapsedSeconds();

  std::cout << "Structs took " << stSecs << " seconds.\nTuples took " << tpSecs << " seconds.\n";

  return 0;
}