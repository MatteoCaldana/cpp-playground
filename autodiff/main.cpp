#include <functional>
#include <cmath>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>

auto timeit(const std::function<void()> &f, size_t N = 1) {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  for(size_t i = 0; i < N; ++i)
    f();
  const auto t1 = high_resolution_clock::now();
  return duration_cast<milliseconds>(t1 - t0).count();
}

namespace autodiff {
  using func_t = std::function<double(double)>;

  class Var {
  public:
    Var(
      const func_t& val,
      const func_t& der
    ) :
      m_val(val), m_der(der) {}

    double eval(double x) const {
      return m_val(x);
    }
    double eval_der(double x) const {
      return m_der(x);
    }

    //======================================================================================
    Var& operator+=(const Var& rhs) // compound assignment (does not need to be a member,
    {                           // but often is, to modify the private members)
        /* addition of rhs to *this takes place here */
      m_val = [m_val = m_val, eval = rhs.m_val](double x) { return m_val(x) + eval(x);};
      m_der = [m_der = m_der, evald = rhs.m_der](double x) { return m_der(x) + evald(x);};
      return *this; // return the result by reference
    }

    // friends defined inside class body are inline and are hidden from non-ADL lookup
    friend Var operator+(Var lhs,        // passing lhs by value helps optimize chained a+b+c
      const Var& rhs) // otherwise, both parameters may be const references
    {
      lhs += rhs; // reuse compound assignment
      return lhs; // return the result by value (uses move constructor)
    }
    //======================================================================================
    Var& operator+=(double rhs) {
      m_val = [m_val = m_val, rhs = rhs](double x) { return m_val(x) + rhs;};
      m_der = [m_der = m_der](double x) { return m_der(x);};
      return *this;
    }
    friend Var operator+(Var lhs, double rhs) {
      lhs += rhs;
      return lhs;
    }
    friend Var operator+(double lhs, Var rhs) {
      rhs += lhs;
      return rhs;
    }
    //======================================================================================
    Var& operator*=(const Var& rhs) {
      // care, order matters, you need to copy m_val before re-assigning its value
      m_der = [m_val = m_val, eval = rhs.m_val, m_der = m_der, evald = rhs.m_der](double x) { 
        return m_der(x) * eval(x) + m_val(x) * evald(x);
      };
      m_val = [m_val = m_val, eval = rhs.m_val](double x) { return m_val(x) * eval(x);};
      return *this;
    }
    friend Var operator*(Var lhs, const Var& rhs) {
      lhs *= rhs;
      return lhs;
    }
    //======================================================================================
    Var& operator*=(double rhs) {
      m_val = [m_val = m_val, rhs = rhs](double x) { return m_val(x) * rhs;};
      m_der = [m_der = m_der, rhs = rhs](double x) { return m_der(x) * rhs;};
      return *this;
    }
    friend Var operator*(Var lhs, double rhs) {
      lhs *= rhs;
      return lhs;
    }
    friend Var operator*(double lhs, Var rhs) {
      rhs *= lhs;
      return rhs;
    }
    //======================================================================================
    Var& operator^=(double rhs) {
      m_der = [m_val = m_val, m_der = m_der, rhs = rhs](double x) { return rhs * std::pow(m_val(x), rhs - 1.0) * m_der(x);};
      m_val = [m_val = m_val, rhs = rhs](double x) { return std::pow(m_val(x), rhs);};
      return *this;
    }
    friend Var operator^(Var lhs, double rhs) {
      lhs ^= rhs;
      return lhs;
    }
    //======================================================================================
    Var operator()(const Var& v) const {
      return {[m_val = m_val, eval = v.m_val](double x) { return m_val(eval(x));}, 
      [m_val = m_val, eval = v.m_val, m_der = m_der, evald = v.m_der](double x) {
        return m_der(eval(x)) * evald(x);
      }};
    }
  private:
    func_t m_val, m_der;
  };

  const Var sin([](double x) {return std::sin(x);}, [](double x) {return std::cos(x);});
  const Var cos([](double x) {return std::cos(x);}, [](double x) {return -std::sin(x);});
  const Var exp([](double x) {return std::exp(x);}, [](double x) {return std::exp(x);});
  const Var x([](double x) {return x;}, [](double x) {return 1.0;});

  using test_t = std::tuple<Var, func_t, func_t>;
  std::pair<double, double> test(const Var& v,
    const func_t& val,
    const func_t& der,
    double min, double max, unsigned n) {
    const auto h = (max - min) / n;
    const auto x_range = std::ranges::views::iota(0u, n) |
      std::ranges::views::transform([=](auto i) { return min + i * h; });
    const std::vector<double> points(x_range.begin(), x_range.end());

    std::vector<double> err_val(n), err_der(n);

    std::transform(points.begin(), points.end(), err_val.begin(), [&](double t) {return std::abs(v.eval(t) - val(t));});
    std::transform(points.begin(), points.end(), err_der.begin(), [&](double t) {return std::abs(v.eval_der(t) - der(t));});

    return { *std::max_element(err_val.cbegin(), err_val.cend()),
             *std::max_element(err_der.cbegin(), err_der.cend()) };
  }
}

template<class Lhs, class Rhs>
class Prod {
public:
  Prod(const Lhs &lhs, const Rhs &rhs) : mlhs(lhs), mrhs(rhs) {}
  double eval(double x) const { return mlhs.eval(x) * mrhs.eval(x); }
  double der(double x) const { return mlhs.der(x) * mrhs.eval(x) + mlhs.eval(x) * mrhs.der(x); }
private:
  const Lhs mlhs;
  const Rhs mrhs;
};

template<class T>
class Sin {
public:
  Sin(const T &t) : mt(t) {}
  double eval(double x) const { return std::sin(mt.eval(x)); }
  double der(double x) const { return std::cos(mt.eval(x)) * mt.der(x); }
private:
  const T mt;
};

class X {
public:
  double eval(double x) const { return x; }
  constexpr double der(double x) const { return 1.0; }
};

template<class Lhs, class Rhs>
inline Prod<Lhs, Rhs> operator*(const Lhs &lhs, const Rhs &rhs) {
  return Prod{lhs, rhs};
}

int main(int argc, char **argv) {
  const size_t N = 10'000'000; 

  const auto f = [](double x) { return std::sin(x*x*x*x*x); };
  const auto df = [](double x) { return 5.0 * x*x*x*x * std::cos(x*x*x*x*x); };

  std::default_random_engine engine(0);
  std::uniform_real_distribution<double> dist(0, 1);

  const auto test_manual = [&]() {
    const auto y = df(dist(engine));
  };

  const auto F = autodiff::sin(autodiff::x*autodiff::x*autodiff::x*autodiff::x*autodiff::x);

  const auto test_autodiff = [&]() {
    const auto y = F.eval_der(dist(engine));
  };

  const X x;
  const auto G = Sin{x*x*x*x*x};

  const auto test_autodiff2 = [&]() {
    const auto y = G.der(dist(engine));
  };

  std::cout << "Test hard coded:               " << timeit(test_manual, N) << "[ms]\n";
  std::cout << "Test autodiff:               " << timeit(test_autodiff, N) << "[ms]\n";
  std::cout << "Test autodiff2:               " << timeit(test_autodiff2, N) << "[ms]\n";
}

