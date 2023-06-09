#pragma once

#include <Eigen/Dense>

template<typename T, int dim>
using Vect = Eigen::Array<T, dim, 1>;

template<typename T, int dim>
struct Particle {
  T mass;
  Vect<T, dim> pos, vel, acc;
};

template<int dim, typename T>
inline typename T::Scalar norm(const T &a, const T &b) {
  typename T::Scalar result = 0;
  for(int i = 0; i < dim; ++i)
    result += (a[i] - b[i])*(a[i] - b[i]);
  return std::sqrt(result);
}
