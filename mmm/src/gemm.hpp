#pragma once
// https://github.com/fangjunzhou/blas-playground/

#include <cblas.h>
#include <omp.h>

template <typename T>
inline __attribute__((always_inline)) void blas_wrapper(const T *A, const T *B,
                                                        T *AB, int N) {
  if constexpr (std::is_same<T, double>::value) {
    cblas_dgemm(
        CblasRowMajor,  // order
        CblasNoTrans,   // transpose A
        CblasNoTrans,   // transpose B
        N,              // Number of rows in matrices A and C.
        N,              // Number of columns in matrices B and C.
        N,    // Number of columns in matrix A; number of rows in matrix B.
        1.0,  // Scaling factor for the product of matrices A and B.
        A,    // A
        N,    // The size of the first dimension of matrix A
        B,    // B
        N,    // The size of the first dimension of matrix B
        1.0,  // Scaling factor for matrix C.
        AB,   // C
        N     // The size of the first dimension of matrix C
    );
  } else if constexpr (std::is_same<T, float>::value) {
    cblas_sgemm(
        CblasRowMajor,  // order
        CblasNoTrans,   // transpose A
        CblasNoTrans,   // transpose B
        N,              // Number of rows in matrices A and C.
        N,              // Number of columns in matrices B and C.
        N,    // Number of columns in matrix A; number of rows in matrix B.
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
}

template <typename T>
void mm_naive(const T *A, const T *B, T *AB, int N) {
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j) {
      T tmp = 0.0;
      for (int k = 0; k < N; ++k) {
        tmp += A[i * N + k] * B[k * N + j];
      }
      AB[i * N + j] = tmp;
    }
}

template <typename T>
void mm_naive_parallel(const T *A, const T *B, T *AB, int N) {
#pragma omp parallel for shared(A, B, AB)
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j) {
      T abij = 0.0;
      for (int k = 0; k < N; ++k) {
        abij += A[i * N + k] * B[k * N + j];
      }
      AB[i * N + j] = abij;
    }
}

template <typename T>
void mm_switch(const T *A, const T *B, T *AB, int N) {
  int NN = N * N;
  for (int i = 0; i < NN; ++i) {
    AB[i] = 0.0;
  }
  for (int i = 0; i < N; ++i)
    for (int k = 0; k < N; ++k)
      for (int j = 0; j < N; ++j) {
        AB[i * N + j] += A[i * N + k] * B[k * N + j];
      }
}

template <typename T>
void mm_switch_parallel(const T *A, const T *B, T *AB, int N) {
  const int NN = N * N;
#pragma omp parallel for shared(AB)
  for (int i = 0; i < NN; ++i) {
    AB[i] = 0.0;
  }
#pragma omp parallel for shared(A, B, AB)
  for (int i = 0; i < N; ++i)
    for (int k = 0; k < N; ++k)
      for (int j = 0; j < N; ++j) {
        AB[i * N + j] += A[i * N + k] * B[k * N + j];
      }
}

template <typename T>
void mm_transpose(const T *A, const T *B, T *AB, int N) {
  T *tmp = new T[N * N];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      tmp[i * N + j] = B[j * N + i];
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      AB[i * N + j] = 0.0;
      for (int k = 0; k < N; k++) {
        AB[i * N + j] += A[i * N + k] * tmp[j * N + k];
      }
    }
  }

  delete[] tmp;
}

template <typename T>
void mm_transpose_parallel(const T *A, const T *B, T *AB, int N) {
  T *tmp = new T[N * N];
#pragma omp parallel for shared(tmp, B)
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      tmp[i * N + j] = B[i + j * N];
    }
  }

#pragma omp parallel for shared(A, B, AB)
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      T abij = 0.0;
      for (int k = 0; k < N; k++) {
        abij += A[i * N + k] * tmp[j * N + k];
      }
      AB[i * N + j] = abij;
    }
  }

  delete[] tmp;
}

template <typename T, int BlockSize>
void mm_blocked(const T *A, const T *B, T *AB, int N) {
  if (N < BlockSize) {
    return mm_blocked<T, 1>(A, B, AB, N);
  }
  const int blockNum = N / BlockSize;
  const int NN = N * N;

  // Clear matC.
  for (int i = 0; i < NN; i++) {
    AB[i] = 0;
  }

  // Traverse blocks.
  for (int bi = 0; bi < blockNum; bi++) {
    for (int bj = 0; bj < blockNum; bj++) {
      for (int bk = 0; bk < blockNum; bk++) {
        // Block GEMM.
        for (int i = 0; i < BlockSize; i++) {
          for (int j = 0; j < BlockSize; j++) {
            int cIdx = bi * BlockSize * blockNum * BlockSize +
                       i * blockNum * BlockSize + bj * BlockSize + j;
            T partial = 0;
            for (int k = 0; k < BlockSize; k++) {
              int aIdx = bi * BlockSize * blockNum * BlockSize +
                         i * blockNum * BlockSize + bk * BlockSize + k;
              int bIdx = bk * BlockSize * blockNum * BlockSize +
                         k * blockNum * BlockSize + bj * BlockSize + j;
              partial += A[aIdx] * B[bIdx];
            }
            AB[cIdx] += partial;
          }
        }
      }
    }
  }
}

template <typename T, int BlockSize>
void mm_blocked_parallel(const T *A, const T *B, T *AB, int N) {
  if (N < BlockSize) {
    return mm_blocked_parallel<T, 1>(A, B, AB, N);
  }
  const int blockNum = N / BlockSize;
  const int NN = N * N;
#pragma omp parallel for shared(AB)
  for (int i = 0; i < NN; ++i) {
    AB[i] = 0.0;
  }
#pragma omp parallel for shared(A, B, AB)
  for (int bi = 0; bi < blockNum; bi++) {
    for (int bj = 0; bj < blockNum; bj++) {
      for (int bk = 0; bk < blockNum; bk++) {
        // Block GEMM
        for (int i = 0; i < BlockSize; ++i) {
          for (int j = 0; j < BlockSize; ++j) {
            const int abij = bi * BlockSize * blockNum * BlockSize +
                             i * blockNum * BlockSize + bj * BlockSize + j;
            T tmp = 0;
            for (int k = 0; k < BlockSize; ++k) {
              const int aij = bi * BlockSize * blockNum * BlockSize +
                              i * blockNum * BlockSize + bk * BlockSize + k;
              const int bij = bk * BlockSize * blockNum * BlockSize +
                              k * blockNum * BlockSize + bj * BlockSize + j;
              tmp += A[aij] * B[bij];
            }
            AB[abij] += tmp;
          }
        }
      }
    }
  }
}

template <typename T, int BlockSize>
void mm_blocked_switch(const T *A, const T *B, T *AB, int N) {
  if (N < BlockSize) {
    return mm_blocked_switch<T, 1>(A, B, AB, N);
  }
  static T localA[BlockSize][BlockSize] alignas(64);
  static T localB[BlockSize][BlockSize] alignas(64);
  static T localC[BlockSize][BlockSize] alignas(64);

  const int blockNum = N / BlockSize;

  // Traverse blocks.
  for (int bi = 0; bi < blockNum; bi++) {
    for (int bj = 0; bj < blockNum; bj++) {
      // Clear localC.
      for (int i = 0; i < BlockSize; i++) {
        for (int j = 0; j < BlockSize; j++) {
          localC[i][j] = 0;
        }
      }

      for (int bk = 0; bk < blockNum; bk++) {
        // Copy local block.
        for (int i = 0; i < BlockSize; i++) {
          for (int j = 0; j < BlockSize; j++) {
            const int aIdx = bi * BlockSize * blockNum * BlockSize +
                             i * blockNum * BlockSize + bk * BlockSize + j;
            const int bIdx = bk * BlockSize * blockNum * BlockSize +
                             i * blockNum * BlockSize + bj * BlockSize + j;
            localA[i][j] = A[aIdx];
            localB[i][j] = B[bIdx];
          }
        }

        // Block GEMM.
        for (int i = 0; i < BlockSize; i++) {
          for (int k = 0; k < BlockSize; k++) {
            for (int j = 0; j < BlockSize; j++) {
              localC[i][j] += localA[i][k] * localB[k][j];
            }
          }
        }
      }

      // Copy localC back.
      for (int i = 0; i < BlockSize; i++) {
        for (int j = 0; j < BlockSize; j++) {
          const int cIdx = bi * BlockSize * blockNum * BlockSize +
                           i * blockNum * BlockSize + bj * BlockSize + j;
          AB[cIdx] = localC[i][j];
        }
      }
    }
  }
}

template <typename T, int BlockSize>
void mm_blocked_switch_parallel(const T *A, const T *B, T *AB, int N) {
  if (N < BlockSize) {
    return mm_blocked_switch_parallel<T, 1>(A, B, AB, N);
  }
  static T localA[BlockSize][BlockSize] alignas(64);
  static T localB[BlockSize][BlockSize] alignas(64);
  static T localC[BlockSize][BlockSize] alignas(64);

  const int blockNum = N / BlockSize;

  // Traverse blocks.
#pragma omp parallel for private(localA, localB, localC)
  for (int bi = 0; bi < blockNum; bi++) {
    for (int bj = 0; bj < blockNum; bj++) {
      // Clear localC.
      for (int i = 0; i < BlockSize; i++) {
        for (int j = 0; j < BlockSize; j++) {
          localC[i][j] = 0;
        }
      }

      for (int bk = 0; bk < blockNum; bk++) {
        // Copy local block.
        for (int i = 0; i < BlockSize; i++) {
          for (int j = 0; j < BlockSize; j++) {
            const int aIdx = bi * BlockSize * blockNum * BlockSize +
                             i * blockNum * BlockSize + bk * BlockSize + j;
            const int bIdx = bk * BlockSize * blockNum * BlockSize +
                             i * blockNum * BlockSize + bj * BlockSize + j;
            localA[i][j] = A[aIdx];
            localB[i][j] = B[bIdx];
          }
        }

        // Block GEMM.
        for (int i = 0; i < BlockSize; i++) {
          for (int k = 0; k < BlockSize; k++) {
#pragma omp simd
            for (int j = 0; j < BlockSize; j++) {
              localC[i][j] += localA[i][k] * localB[k][j];
            }
          }
        }
      }

      // Copy localC back.
      for (int i = 0; i < BlockSize; i++) {
        for (int j = 0; j < BlockSize; j++) {
          const int cIdx = bi * BlockSize * blockNum * BlockSize +
                           i * blockNum * BlockSize + bj * BlockSize + j;
          AB[cIdx] = localC[i][j];
        }
      }
    }
  }
}

template <class T>
using fnType = void (*)(const T *, const T *, T *, int);

static constexpr fnType<double> mmmTableDouble[] = {
    blas_wrapper<double>,
    mm_naive<double>,
    mm_naive_parallel<double>,
    mm_switch<double>,
    mm_switch_parallel<double>,
    mm_transpose<double>,
    mm_transpose_parallel<double>,
    mm_blocked<double, 8>,
    mm_blocked<double, 16>,
    mm_blocked_parallel<double, 8>,
    mm_blocked_parallel<double, 16>,
    mm_blocked_switch<double, 8>,
    mm_blocked_switch<double, 16>,
    mm_blocked_switch_parallel<double, 8>,
    mm_blocked_switch_parallel<double, 16>,
    mm_blocked_switch_parallel<double, 32>};
static constexpr fnType<float> mmmTableFloat[] = {
    blas_wrapper<float>,
    mm_naive<float>,
    mm_naive_parallel<float>,
    mm_switch<float>,
    mm_switch_parallel<float>,
    mm_transpose<float>,
    mm_transpose_parallel<float>,
    mm_blocked<float, 8>,
    mm_blocked<float, 16>,
    mm_blocked_parallel<float, 8>,
    mm_blocked_parallel<float, 16>,
    mm_blocked_switch<float, 8>,
    mm_blocked_switch<float, 16>,
    mm_blocked_switch_parallel<float, 8>,
    mm_blocked_switch_parallel<float, 16>,
    mm_blocked_switch_parallel<float, 32>};

constexpr const char *mmmTableNames[] = {
    "oblas",     "naive",      "p+naive",     "switch",
    "p+switch",  "trans",      "p+trans",     "block8",
    "block16",   "p+block8",   "p+block16",   "s+block8",
    "s+block16", "p+s+block8", "p+s+block16", "p+s+block32"};
static_assert(sizeof(mmmTableFloat) == sizeof(mmmTableDouble));
static constexpr size_t nFunc = sizeof(mmmTableFloat) / sizeof(fnType<float>);

template <typename T>
inline constexpr auto get_table() {
  if constexpr (std::is_same<T, double>::value)
    return mmmTableDouble;
  else if constexpr (std::is_same<T, float>::value)
    return mmmTableFloat;
}
