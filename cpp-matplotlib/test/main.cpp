#include "cppmatplot.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

int main() {
  pyplot plt;
  /////////////////////////////////////////////////////////////////////////////
  std::mt19937 mersenne_engine{0};
  std::uniform_real_distribution<double> dist{0.0, 1.0};
  /////////////////////////////////////////////////////////////////////////////
  {
    std::vector<double> signal(1024);
    for (size_t i = 0; i < signal.size(); ++i)
      signal[i] = std::exp(i / -256.0) * std::cos(2 * M_PI * 8 * i / 1024.0);
    plt.plot(signal, "color"_a = "b", "linewidth"_a = 2.0);
    plt.grid("on");
    plt.savefig("gallery01.png");
    plt.close();
  }
  /////////////////////////////////////////////////////////////////////////////
  {
    std::vector<double> x(20), y(20), sizes(20);
    std::generate(std::begin(x), std::end(x),
                  [&] { return dist(mersenne_engine); });
    std::generate(std::begin(y), std::end(y),
                  [&] { return dist(mersenne_engine); });
    std::generate(std::begin(sizes), std::end(sizes),
                  [&] { return 200.0 * dist(mersenne_engine); });
    plt.scatter(x, y, "s"_a = sizes, "c"_a = sizes);
    plt.savefig("gallery02.png");
    plt.close();
  }
  /////////////////////////////////////////////////////////////////////////////
  {
    std::vector<double> x(20), y(20), yl(20), yu(20);
    std::iota(x.begin(), x.end(), 0.0);
    std::generate(std::begin(y), std::end(y),
                  [&] { return std::exp(3.0 * dist(mersenne_engine)); });
    std::generate(std::begin(yl), std::end(yl),
                  [&] { return std::exp(dist(mersenne_engine)); });
    std::generate(std::begin(yu), std::end(yu),
                  [&] { return std::exp(30.0 * dist(mersenne_engine)); });
    plt.fill_between(x, yl, yu, "alpha"_a=0.5);
    plt.plot(x, y, "k--", "linewidth"_a = 2.0);
    plt.yscale("log");
    std::vector ylim = {0.1, 1e13};
    plt.ylim(ylim);
    plt.savefig("gallery03.png");
    plt.close();
  }
  /////////////////////////////////////////////////////////////////////////////
  {
    std::vector<double> x(200), y(200), z(200);
    std::generate(std::begin(x), std::end(x),
                  [&] { return 6.0 * (dist(mersenne_engine) - 0.5); });
    std::generate(std::begin(y), std::end(y),
                  [&] { return 6.0 * (dist(mersenne_engine) - 0.5); });
    for(size_t i = 0; i < z.size(); ++i)
      z[i] = (1 - x[i]/2. + std::pow(x[i], 5.) + std::pow(y[i], 3.)) * std::exp(-x[i]*x[i] - y[i]*y[i]);
    plt.plot(x, y, 'o', "markersize"_a=2, "color"_a="grey");
    plt.tricontourf(x, y, z, "cmap"_a=plt.jet());
    plt.savefig("gallery04.png");
    plt.close();

  }

  return 0;
}