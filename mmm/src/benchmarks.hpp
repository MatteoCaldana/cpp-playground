#pragma once

#include <chrono>
#include <random>
#include <vector>

#include "gemm.hpp"

std::pair<double, double> mean_std(const std::vector<uint64_t> &data) {
  double sum = 0.0;
  for (const auto &value : data) {
    sum += value;
  }

  double mean = sum / data.size();

  double squaredSum = 0.0;
  for (const auto &value : data) {
    squaredSum += (value - mean) * (value - mean);
  }

  double variance = squaredSum / data.size();
  double standardDeviation = std::sqrt(variance);

  return std::make_pair(mean, standardDeviation);
}

template <typename T>
void run_benchamrks(int max_size, int reps) {
  std::cout << "size";
  for (size_t i = 0; i < nFunc; ++i) {
    std::cout << "," << mmmTableNames[i];
  }
  std::cout << "\n";

  std::mt19937 gen(0);
  std::uniform_real_distribution<T> dis(-10.0, 10.0);

  constexpr static auto table = get_table<T>();
  for (int i = 1; i < max_size; ++i) {
    const int size = 1 << i;
    std::cout << size;

    const auto n = size * size;
    std::vector<T> A(n), B(n), AB(n);
    for (int i = 0; i < n; ++i) {
      A[i] = dis(gen);
      B[i] = dis(gen);
    }

    for (size_t j = 0; j < nFunc; ++j) {
      std::vector<uint64_t> measures;
      for (int r = 0; r < reps; ++r) {
        using namespace std::chrono;
        const auto t0 = high_resolution_clock::now();
        table[j](A.data(), B.data(), &AB[0], size);
        const auto t1 = high_resolution_clock::now();
        const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
        measures.push_back(dt);
      }
      const auto ms = mean_std(measures);
      std::cout << "," << ms.first << ";" << ms.second;
    }
    std::cout << "\n";
  }
}