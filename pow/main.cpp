#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <cmath>

using base_t = double;

base_t myPow(base_t x, unsigned n) {
  base_t res = 1;
  while (n > 0) {
    if (n & 1)
      res *= x;
    x *= x;
    n >>= 1;
  }
  return res;
}

base_t myPow2(base_t x, unsigned n) {
  base_t r = 1;
  while (n > 0) {
    r *= 1 + (n & 1) * (x - 1);
    x *= x;
    n >>= 1;
  }
  return r;
}

base_t myPow3(base_t x, unsigned n) {
  base_t r = 1;
  while (n > 0) {
    const auto b = n & 1;
    r *= b + x * (1 - b);
    x *= x;
    n >>= 1;
  }
  return r;
}


base_t naivePow(base_t x, unsigned n) {
  base_t result = 1;
  while(n--) { result *= x; }
  return result;
}


void test(base_t (*func)(base_t, unsigned),
          base_t* buffer,
          const base_t* base,
          const unsigned* exp,
          size_t N) {
  for (size_t i = 0; i < N; ++i) {
    func(1.1, 10);
  }
}

int main() {
  constexpr size_t N = 10'000'000;
  base_t buffer[N], base[N];
  unsigned exp[N];

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<base_t> dist_base(0, 10);
  std::uniform_int_distribution<unsigned> dist_exp(2, 100);

  for (size_t i = 0; i < N; ++i) {
    base[i] = dist_base(mt);
    exp[i] = dist_exp(mt);
  }

  using namespace std::chrono;
  for (auto f : {naivePow, myPow, myPow2, myPow3, std::pow}) {
    auto t0 = steady_clock::now();
    test(f, buffer, base, exp, N);
    auto t1 = steady_clock::now();
    std::cout << "Elapsed time = "
              << duration_cast<microseconds>(t1 - t0).count() << "[µs]"
              << std::endl;
  }

  return 0;
}