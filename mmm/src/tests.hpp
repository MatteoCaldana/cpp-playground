#pragma once

#include <limits>
#include <random>

#include "gemm.hpp"

template <typename T>
bool check_close(const T *lhs, const T *rhs, int N) {
  for (int i = 0; i < N; ++i) {
    if (std::abs(lhs[i] - rhs[i]) >
        100.0 * N * std::numeric_limits<T>::epsilon()) {
      std::cout << std::abs(lhs[i] - rhs[i]) << std ::endl;
      return false;
    }
  }
  return true;
}

template <typename T>
void run_tests() {
  std::mt19937 gen(0);
  std::uniform_real_distribution<T> dis(-10.0, 10.0);

  const std::vector sizes = {2, 4, 8, 16, 32, 64, 128, 256, 512};
  for (int size : sizes) {
    std::cout << "Testing size: " << size << std::endl;
    std::cout << "  Initializing random matrices" << std::endl;

    const auto n = size * size;
    std::vector<T> A(n), B(n), AB(n), AB_blas(n);
    for (int i = 0; i < n; ++i) {
      A[i] = dis(gen);
      B[i] = dis(gen);
    }
    std::cout << "  Initializing reference solution" << std::endl;
    blas_wrapper<T>(A.data(), B.data(), &AB_blas[0], size);

    constexpr static auto table = get_table<T>();
    for (size_t i = 1; i < nFunc; ++i) {
      std::cout << "    Checking: " << mmmTableNames[i] << std::endl;
      table[i](A.data(), B.data(), &AB[0], size);
      if (!check_close<T>(AB.data(), AB_blas.data(), n)) {
        std::cout << "Error: large discrepancy" << std::endl;
        std::exit(-1);
      }
    }
  }
}