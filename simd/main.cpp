// check your cpu flags: cat /proc/cpuinfo | grep flags
// docs: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
// references:
// - https://leimao.github.io/blog/SSE-AVX-SIMD-Vectorization-Intrinsics/
// - https://chryswoods.com/vector_c++/emmintrin.html

// compile with: g++ main.cpp -std=c++20 -O3 -msse2 -mavx -Wall

#include <array>
#include <emmintrin.h>
#include <immintrin.h>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <type_traits>

union f64x4 {
  __m256d  m;
  double f[4];
  inline static constexpr size_t size = 4;
  using scalar_type = double;
};

union f64x2 {
  __m128d  m;
  double f[2];
  inline static constexpr size_t size = 2;
  using scalar_type = double;
};

union f32x8 {
  __m256  m;
  float f[8];
  inline static constexpr size_t size = 8;
  using scalar_type = float;
};

union f32x4 {
  __m128  m;
  float f[4];
  inline static constexpr size_t size = 4;
  using scalar_type = float;
};

enum class VectorizationType { SSE128, AVX256 };

template<class T>
concept IsVectorizable = std::is_same<T, double>::value || std::is_same<T, float>::value;

template<class T>
concept IsVectorized = std::is_same<T, f64x4>::value || std::is_same<T, f64x2>::value || std::is_same<T, f32x8>::value || std::is_same<T, f32x4>::value;


template<IsVectorizable Real, VectorizationType Size>
auto _init(const std::vector<Real> &v) {
  if constexpr (std::is_same<Real, double>::value) {
    if constexpr (Size == VectorizationType::SSE128) {
      return std::vector<f64x2>(v.size() / 2);
    } else {
      return std::vector<f64x4>(v.size() / 4);
    }
  } else {
    if constexpr (Size == VectorizationType::SSE128) {
      return std::vector<f32x4>(v.size() / 4);
    } else {
      return std::vector<f32x8>(v.size() / 8);
    }
  }
}

template<IsVectorizable Real, VectorizationType Size>
auto vectorize(const std::vector<Real> &v) {
  auto vv = _init<Real, Size>(v);
  for (size_t i = 0; i < v.size(); ++i)
    vv[i / decltype(vv)::value_type::size].f[i % decltype(vv)::value_type::size] = v[i];
  return vv;
}

template<IsVectorized T>
inline auto to_real(const T &v) {
  typename T::scalar_type s = 0;
  for(size_t i = 0; i < T::size; ++i)
    s += v.f[i];
  return s;
}

template<IsVectorized T>
inline auto vectorized_sum(T &a, T &b) {
  if constexpr (std::is_same<T, f64x4>::value) {
    return _mm256_add_pd(a.m, b.m);
  } else if constexpr (std::is_same<T, f64x2>::value) {
    return _mm_add_pd(a.m, b.m);
  } else if constexpr (std::is_same<T, f32x8>::value) {
    return _mm256_add_ps(a.m, b.m);
  } else {
    return _mm_add_ps(a.m, b.m);
  }
}

using namespace std::chrono;
int main() {
  const unsigned N = 100'000'000;
  using real = float;

  std::default_random_engine engine{0};
  std::uniform_real_distribution<real> dist{0, 1};
  auto gen = [&dist, &engine]() {return dist(engine);};
  std::vector<real> v(N);
  std::generate(std::begin(v), std::end(v), gen);

  {
    auto t1 = high_resolution_clock::now();
    real sum = std::accumulate(v.begin(), v.end(), 0.0);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    std::cout << ms_int.count() << "ms\n";
    std::cout << sum << std::endl;
  }


  auto vv = vectorize<real, VectorizationType::AVX256>(v);
  {
    auto t1 = high_resolution_clock::now();
    decltype(vv)::value_type sum = { 0 };
    for (size_t i = 0; i < vv.size(); ++i)
      sum.m = vectorized_sum(sum, vv[i]);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    std::cout << ms_int.count() << "ms\n";
    std::cout << to_real(sum) << std::endl;
  }

  return 0;
}