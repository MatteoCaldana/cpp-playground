#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <set>
#include <vector>

template <typename T>
class Fraction {
 public:
  Fraction(T num, T den = 1)
      : m_GCD(Fraction<T>::gcd(num, den)),
        m_num(num / m_GCD),
        m_den(den / m_GCD) {
    if (m_den < 0) {
      m_num *= -1;
      m_den *= -1;
    }
  }
  T num() const { return m_num; }
  T den() const { return m_den; }
  double eval() const { return double(m_num) / double(m_den); }

  Fraction& operator+=(const Fraction& rhs) {
    m_num = m_num * rhs.m_den + rhs.m_num * m_den;
    m_den *= rhs.m_den;
    return *this;
  }

  Fraction& operator*=(const Fraction& rhs) {
    m_num *= rhs.m_num;
    m_den *= rhs.m_den;
    return *this;
  }

  Fraction& operator/=(const Fraction& rhs) {
    m_num *= rhs.m_den;
    m_den *= rhs.m_num;
    return *this;
  }

  friend Fraction operator+(Fraction lhs, const Fraction& rhs) {
    lhs += rhs;
    return lhs;
  }

  friend Fraction operator*(Fraction lhs, const Fraction& rhs) {
    lhs *= rhs;
    return lhs;
  }

  friend Fraction operator/(Fraction lhs, const Fraction& rhs) {
    lhs /= rhs;
    return lhs;
  }

  friend bool operator<(const Fraction& l, const Fraction& r) {
    return l.m_num * r.m_den < l.m_den * r.m_num;
  }

  friend std::ostream& operator<<(std::ostream& out, const Fraction& f) {
    return out << f.num() << '/' << f.den();
  }

 private:
  T m_GCD, m_num, m_den;
  static T gcd(T a, T b) {
    while (b != 0) {
      T t = b;
      b = a % b;
      a = t;
    }
    return a;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  if (!v.empty()) {
    out << '[';
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";  // use two ANSI backspace characters '\b' to overwrite
  }
  return out;
}

auto timeit(const std::function<void()>& f, const std::string& testname = "") {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  f();
  const auto t1 = high_resolution_clock::now();
  const auto dt = duration_cast<milliseconds>(t1 - t0).count();
  std::cout << "Elapsed: " << dt << " [ms]" << (testname.size() ? "for " : "")
            << testname << std::endl;
  return dt;
}

// TODO: optimize types
using e_t = unsigned char;  // mesh index type
using f_t = size_t;         // fraction element type
using s_t = int;            // solution index type
using Frac = Fraction<f_t>;

std::vector<std::vector<Frac>> get_meshes(f_t N) {
  std::vector<std::vector<Frac>> v;
  v.reserve(N - 1);
  for (auto i = 2ull; i < N + 1; ++i) {
    v.emplace_back();
    v.back().reserve(i + 1);
    for (auto j = 0ull; j <= i; ++j) {
      v.back().emplace_back(j, i);
    }
  }
  return v;
}

std::vector<Frac> get_partitions(const std::vector<std::vector<Frac>>& meshes) {
  std::set<Frac> s;
  for (const auto& lvl : meshes) {
    for (const auto& e : lvl) {
      s.insert(e);
    }
  }
  return std::vector<Frac>{s.begin(), s.end()};
  ;
}

template <typename T, typename I>
class Matrix {
 public:
  constexpr Matrix(I r, I c = 1) : rows(r), cols(c) {
    data = new T[rows * cols];
  }
  ~Matrix() { delete[] data; }
  const I rows, cols;
  constexpr T& operator()(I x, I y) { return data[y * rows + x]; }
  constexpr const T& operator()(I x, I y) const { return data[y * rows + x]; }

  friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {
    for (I i = 0; i < m.rows; ++i) {
      out << (i == 0 ? "[[" : " [");
      for (I j = 0; j < m.cols; ++j) {
        out << int(m(i, j)) << ", ";
      }
      out << "\b\b]";
      if (i < m.rows - 1)
        out << "\n";
    }
    out << "]";
    return out;
  }

 private:
  T* data;
};

Matrix<e_t, s_t> get_lvls_idx(const std::vector<Frac>& partitions,
                              const std::vector<std::vector<Frac>>& meshes) {
  Matrix<e_t, s_t> lvls_idx(meshes.size(), partitions.size() - 1);
  for (s_t i = 0; i < lvls_idx.rows; ++i) {
    lvls_idx(i, 0) = 1;
  }
  for (s_t i = 2; i < lvls_idx.cols + 1; ++i) {
    for (s_t j = 0; j < lvls_idx.rows; ++j) {
      lvls_idx(j, i - 1) =
          lvls_idx(j, i - 2) + (meshes[j][lvls_idx(j, i - 2)] < partitions[i]);
    }
  }
  return lvls_idx;
}

std::array<s_t, 2> find_closest_upper_and_lower(
    s_t idx,
    const std::vector<s_t>& solution,
    size_t depth1) {
  int lower = -1, upper = -1;
  for (size_t i = 0; i < depth1; ++i) {
    if (solution[i] < idx) {
      if ((lower == -1) || (lower < solution[i]))
        lower = solution[i];
    }
    if (solution[i] > idx) {
      if ((upper == -1) || (upper > solution[i]))
        upper = solution[i];
    }
  }
  return {lower, upper};
}

bool contains(const std::vector<s_t>& solution, s_t idx, size_t depth1) {
  for (size_t i = 0; i < depth1; ++i) {
    if (solution[i] == idx)
      return true;
  }
  return false;
}

void search_from_elem_idx(s_t idx,
                          const Matrix<e_t, s_t>& lvls_idx,
                          s_t depth,
                          std::vector<s_t>& solution,
                          std::vector<std::vector<s_t>>& solutions) {
  // TODO: avoid recursive
  bool discard = false;
  solution[depth] = idx;
  if (depth > 1) {
    const auto up_lo = find_closest_upper_and_lower(idx, solution, depth + 1);
    for (size_t i = 0; i < 2; ++i) {
      if (up_lo[i] > -1) {
        for (s_t j = depth - 1; j < lvls_idx.rows; ++j) {
          if (lvls_idx(j, idx) == lvls_idx(j, up_lo[i])) {
            discard = true;
            break;
          }
        }
      }
    }
  }
  if (!discard) {
    if (depth == lvls_idx.rows) {
      // std::cout << "Resolved: " << solution << std::endl;
      solutions.push_back(solution);
    } else {
      for (s_t i = 0; i < lvls_idx.cols; ++i) {
        if ((lvls_idx(depth, solution[depth]) != lvls_idx(depth, i)) &&
            (!contains(solution, i, depth + 1))) {
          search_from_elem_idx(i, lvls_idx, depth + 1, solution, solutions);
        }
      }
    }
  }
  solution[depth] = -1;
  return;
}

std::vector<Frac> solution_idx_to_solution_pt(
    const std::vector<s_t>& solution,
    const std::vector<Frac>& partitions) {
  std::vector<Frac> sol_pt;
  sol_pt.reserve(solution.size());
  for (const auto& i : solution)
    sol_pt.push_back((partitions[i] + partitions[i + 1]) / 2);
  return sol_pt;
}

bool check_solution_lvl(const std::vector<Frac>& solution,
                        const std::vector<Frac>& mesh) {
  std::vector<Frac> sort_solution = solution;
  std::sort(sort_solution.begin(), sort_solution.begin() + mesh.size() - 1);
  if (sort_solution[0] < 0)
    return false;
  for (size_t i = 1; i < mesh.size(); ++i) {
    if (mesh[i] < sort_solution[i - 1])
      return false;
  }
  return true;
}

bool check_solution(const std::vector<Frac>& solution,
                    const std::vector<std::vector<Frac>>& meshes) {
  for (const auto& mesh : meshes) {
    if (!check_solution_lvl(solution, mesh))
      return false;
  }
  return true;
}

int main() {
  const f_t N = 12;
  const auto meshes = get_meshes(N);
  const auto partitions = get_partitions(meshes);

  std::cout << partitions << std::endl;
  const auto lvls_idx = get_lvls_idx(partitions, meshes);
  std::cout << lvls_idx << std::endl;

  std::vector<s_t> solution(N, -1);
  std::vector<std::vector<s_t>> solutions;
  timeit([&]() { search_from_elem_idx(0, lvls_idx, 0, solution, solutions); });
  std::cout << "Found " << solutions.size() << " solutions" << std::endl;

  // for (const auto& sol : solutions) {
  //   const auto sol_pt = solution_idx_to_solution_pt(sol, partitions);
  //   std::cout << sol << " check: " << check_solution(sol_pt, meshes)
  //             << std::endl;
  // }
  return 0;
}