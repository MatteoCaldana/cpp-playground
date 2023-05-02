#include <cblas.h>

#include <chrono>
#include <cstdlib>  // atoll
#include <iostream>
#include <random>

// https://marek.ai/matrix-multiplication-on-cpu.html
// https://coffeebeforearch.github.io/2020/06/23/mmul.html
// https://siboehm.com/articles/22/Fast-MMM-on-CPU

using namespace std::chrono;

constexpr size_t default_matrix_size = 1024;

inline void blas_wrapper(const double *A, const double *B, double *AB, int N) {
  cblas_dgemm(CblasRowMajor,  // order
              CblasNoTrans,   // transpose A
              CblasNoTrans,   // transpose B
              N,              // Number of rows in matrices A and C.
              N,              // Number of columns in matrices B and C.
              N,  // Number of columns in matrix A; number of rows in matrix B.
              1.0,  // Scaling factor for the product of matrices A and B.
              A,    // A
              N,    // The size of the first dimension of matrix A
              B,    // B
              N,    // The size of the first dimension of matrix B
              1.0,  // Scaling factor for matrix C.
              AB,   // C
              N     // The size of the first dimension of matrix C
  );
}

void mm_naive(const double *A, const double *B, double *AB, int N) {
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k) {
        AB[i * N + j] += A[i * N + k] * B[k * N + j];
      }
}

void mm_naive_tmp(const double *A, const double *B, double *AB, int N) {
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j) {
      double tmp = 0.0;
      for (int k = 0; k < N; ++k) {
        tmp += A[i * N + k] * B[k * N + j];
      }
      AB[i * N + j] = tmp;
    }
}

void mm_naive_switch(const double *A, const double *B, double *AB, int N) {
  for (int i = 0; i < N; ++i)
    for (int k = 0; k < N; ++k)
      for (int j = 0; j < N; ++j) {
        AB[i * N + j] += A[i * N + k] * B[k * N + j];
      }
}

template <int T>
void mm_blocked(const double *A, const double *B, double *C, int N) {
  const int M = N;
  const int K = N;
  for (int m = 0; m < M; m += T) {
    for (int n = 0; n < N; n += T) {
      for (int k = 0; k < K; k += T) {
        const int minMt = std::min(m + T, M);
        const int minNt = std::min(n + T, N);
        const int minKt = std::min(k + T, K);
        for (int mt = m; mt < minMt; mt++) {
          for (int nt = n; nt < minNt; nt++) {
            for (int kt = k; kt < minKt; kt++) {
              C[mt * M + nt] += A[mt * M + kt] * B[kt * K + nt];
            }
          }
        }
      }
    }
  }
}

template <int T>
void mm_blocked_switch(const double *A, const double *B, double *C, int N) {
  const int M = N;
  const int K = N;
  for (int m = 0; m < M; m += T) {
    for (int n = 0; n < N; n += T) {
      for (int k = 0; k < K; k += T) {
        const int minMt = std::min(m + T, M);
        const int minNt = std::min(n + T, N);
        const int minKt = std::min(k + T, K);
        for (int mt = m; mt < minMt; mt++) {
          for (int kt = k; kt < minKt; kt++) {
            for (int nt = n; nt < minNt; nt++) {
              C[mt * M + nt] += A[mt * M + kt] * B[kt * K + nt];
            }
          }
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  const size_t N = argc > 1 ? std::atoll(argv[1]) : default_matrix_size;
  std::cout << "matrix_size = " << N << std::endl;

  for (const auto &f : {blas_wrapper, mm_naive, mm_naive_tmp, mm_naive_switch,
                        mm_blocked<64>, mm_blocked_switch<64>}) {
    std::default_random_engine engine(std::random_device{}());
    std::uniform_real_distribution<double> rand(0, 1);
    double *A = new double[N * N];
    double *B = new double[N * N];
    double *AB = new double[N * N];
    double *C = new double[N * N];

    for (size_t i = 0; i < N; ++i)
      for (size_t j = 0; j < N; ++j) {
        A[i + j * N] = rand(engine);
        B[i + j * N] = rand(engine);
      }

    const auto t0 = high_resolution_clock::now();
    f(A, B, AB, N);
    const auto t1 = high_resolution_clock::now();

    blas_wrapper(A, B, C, N);

    for (size_t i = 0; i < N; ++i)
      for (size_t j = 0; j < N; ++j) {
        if (std::abs(AB[i + j * N] - C[i + j * N]) > 1e-8) {
          std::cout << "Err\n";
        }
      }
    const auto dt = duration_cast<milliseconds>(t1 - t0).count();
    std::cout << "Multiply time: " << dt << " [ms] " << std::endl;
  }
  return 0;
}