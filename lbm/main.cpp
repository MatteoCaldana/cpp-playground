#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

#include "Eigen/Core"
#include "unsupported/Eigen/CXX11/Tensor"

//#include "gnuplot-iostream.h"


using ll = long long;
using Array2 = Eigen::Matrix<double, -1, -1, Eigen::RowMajor>;
using Array2x2d = Eigen::Matrix<double, 2, -1, Eigen::RowMajor>;
using Array3 = Eigen::Tensor<double, 3, Eigen::RowMajor>;

constexpr ll Q = 9;
// # D2Q9 Velocities
// constexpr double C[2][Q] = {{0., 1., -1., 0., 0., 1., -1., -1., 1.},
//                            {0., 0., 0., 1., -1., 1., -1, 1., -1.}};
constexpr double C[Q][2] = {
    {0., 0.}, {1., 0.},   {-1., 0.}, {0., 1.},  {0., -1.},
    {1., 1.}, {-1., -1.}, {-1., 1.}, {1., -1.},
};
// # Weights
constexpr double W[Q] = {
    4. / 9.,  1. / 9.,  1. / 9.,  1. / 9.,  1. / 9.,
    1. / 36., 1. / 36., 1. / 36., 1. / 36.,
};
// # Array for bounce-back
constexpr ll Ns[Q] = {0, 2, 1, 4, 3, 6, 5, 8, 7};

void set_inlets(double sigma, double u_lbm, ll it, Array2x2d &u_top,
                Array2x2d &u_bot, Array2x2d &u_left, Array2x2d &u_right) {
  const double ret =
      1.0 - std::exp(-static_cast<double>(it * it) / (2.0 * sigma * sigma));
  for (ll i = 0; i < u_top.cols(); ++i) {
    u_top(0, i) = u_lbm * ret;
    u_bot(0, i) = 0.0;
    u_left(1, i) = 0.0;
    u_right(1, i) = 0.0;
  }
}

void compute_equilibrium(const Array3 &u, const Array2 &rho, Array3 &g_eq) {
  for (ll q = 0; q < Q; ++q) {
    for (ll i = 0; i < rho.rows(); ++i) {
      for (ll j = 0; j < rho.cols(); ++j) {
        const auto v =
            1.5 * (u(0, i, j) * u(0, i, j) + u(1, i, j) * u(1, i, j));
        const auto t = 3.0 * (u(0, i, j) * C[q][0] + u(1, i, j) * C[q][1]);
        g_eq(q, i, j) = (1.0 + t + 0.5 * t * t - v) * rho(i, j) * W[q];
      }
    }
  }
}

void compute_macroscopic(const Array3 &g, Array3 &u, Array2 &rho) {
  // TODO: rewrite with cache alignment
  for (ll i = 0; i < rho.rows(); ++i) {
    for (ll j = 0; j < rho.cols(); ++j) {
      rho(i, j) = 0.0;
      u(0, i, j) = 0.0;
      u(1, i, j) = 0.0;
      for (ll q = 0; q < Q; ++q) {
        rho(i, j) += g(q, i, j);
        u(0, i, j) += C[q][0] * g(q, i, j);
        u(1, i, j) += C[q][1] * g(q, i, j);
      }
      u(0, i, j) /= rho(i, j);
      u(1, i, j) /= rho(i, j);
    }
  }
}

void collision_and_streaming(double om_p, double om_m, ll lx, ll ly,
                             const Array3 &g_eq, Array3 &g, Array3 &g_up) {
  // Take care of q=0 first
  for (ll i = 0; i < lx + 1; ++i) {
    for (ll j = 0; j < ly + 1; ++j) {
      g_up(0, i, j) = (1.0 - om_p) * g(0, i, j) + om_p * g_eq(0, i, j);
      g(0, i, j) = g_up(0, i, j);
    }
  }

  // Collide other indices
  for (ll q = 0; q < Q; ++q) {
    const auto qb = Ns[q];
    for (ll i = 0; i < lx + 1; ++i) {
      for (ll j = 0; j < ly + 1; ++j) {
        g_up(q, i, j) = (1.0 - 0.5 * (om_p + om_m)) * g(q, i, j) -
                        0.5 * (om_p - om_m) * g(qb, i, j) +
                        0.5 * (om_p + om_m) * g_eq(q, i, j) +
                        0.5 * (om_p - om_m) * g_eq(qb, i, j);
      }
    }
  }

  // Stream
  for (ll i = 0; i < lx; ++i) {
    for (ll j = 0; j < ly; ++j) {
      g(1, i + 1, j) = g_up(1, i, j);
      g(2, i, j) = g_up(2, i + 1, j);
      g(3, i, j + 1) = g_up(3, i, j);
      g(4, i, j) = g_up(4, i, j + 1);
      g(5, i + 1, j + 1) = g_up(5, i, j);
      g(6, i, j) = g_up(6, i + 1, j + 1);
      g(7, i, j + 1) = g_up(7, i + 1, j);
      g(8, i + 1, j) = g_up(8, i, j + 1);
    }
  }

  for (ll i = 0; i < lx; ++i) {
    g(1, i + 1, ly) = g_up(1, i, ly);
    g(2, i, ly) = g_up(2, i + 1, ly);
  }

  for (ll j = 0; j < ly; ++j) {
    g(3, lx, j + 1) = g_up(3, lx, j);
    g(4, lx, j) = g_up(4, lx, j + 1);
  }
}

void zou_he_bottom_left_corner_velocity(Array3 &u, Array2 &rho, Array3 &g) {
  u(0, 0, 0) = u(0, 1, 0);
  u(1, 0, 0) = u(1, 1, 0);

  rho(0, 0) = rho(1, 0);

  g(1, 0, 0) = g(2, 0, 0) + (2.0 / 3.0) * rho(0, 0) * u(0, 0, 0);

  g(3, 0, 0) = g(4, 0, 0) + (2.0 / 3.0) * rho(0, 0) * u(1, 0, 0);

  g(5, 0, 0) = g(6, 0, 0) + (1.0 / 6.0) * rho(0, 0) * u(0, 0, 0) +
               (1.0 / 6.0) * rho(0, 0) * u(1, 0, 0);

  g(7, 0, 0) = 0.0;
  g(8, 0, 0) = 0.0;

  g(0, 0, 0) = rho(0, 0) - g(1, 0, 0) - g(2, 0, 0) - g(3, 0, 0) - g(4, 0, 0) -
               g(5, 0, 0) - g(6, 0, 0) - g(7, 0, 0) - g(8, 0, 0);
}

void zou_he_top_left_corner_velocity(ll ly, Array3 &u, Array2 &rho, Array3 &g) {
  u(0, 0, ly) = u(0, 1, ly);
  u(1, 0, ly) = u(1, 1, ly);

  rho(0, ly) = rho(1, ly);

  g(1, 0, ly) = g(2, 0, ly) + (2.0 / 3.0) * rho(0, ly) * u(0, 0, ly);

  g(4, 0, ly) = g(3, 0, ly) - (2.0 / 3.0) * rho(0, ly) * u(1, 0, ly);

  g(8, 0, ly) = g(7, 0, ly) + (1.0 / 6.0) * rho(0, ly) * u(0, 0, ly) -
                (1.0 / 6.0) * rho(0, ly) * u(1, 0, ly);

  g(5, 0, ly) = 0.0;
  g(6, 0, ly) = 0.0;

  g(0, 0, ly) = rho(0, ly) - g(1, 0, ly) - g(2, 0, ly) - g(3, 0, ly) -
                g(4, 0, ly) - g(5, 0, ly) - g(6, 0, ly) - g(7, 0, ly) -
                g(8, 0, ly);
}

void zou_he_top_right_corner_velocity(ll lx, ll ly, Array3 &u, Array2 &rho,
                                      Array3 &g) {
  u(0, lx, ly) = u(0, lx - 1, ly);
  u(1, lx, ly) = u(1, lx - 1, ly);

  rho(lx, ly) = rho(lx - 1, ly);

  g(2, lx, ly) = g(1, lx, ly) - (2.0 / 3.0) * rho(lx, ly) * u(0, lx, ly);

  g(4, lx, ly) = g(3, lx, ly) - (2.0 / 3.0) * rho(lx, ly) * u(1, lx, ly);

  g(6, lx, ly) = g(5, lx, ly) - (1.0 / 6.0) * rho(lx, ly) * u(0, lx, ly) -
                 (1.0 / 6.0) * rho(lx, ly) * u(1, lx, ly);

  g(7, lx, ly) = 0.0;
  g(8, lx, ly) = 0.0;

  g(0, lx, ly) = rho(lx, ly) - g(1, lx, ly) - g(2, lx, ly) - g(3, lx, ly) -
                 g(4, lx, ly) - g(5, lx, ly) - g(6, lx, ly) - g(7, lx, ly) -
                 g(8, lx, ly);
}

void zou_he_bottom_right_corner_velocity(ll lx, Array3 &u, Array2 &rho,
                                         Array3 &g) {
  u(0, lx, 0) = u(0, lx - 1, 0);
  u(1, lx, 0) = u(1, lx - 1, 0);

  rho(lx, 0) = rho(lx - 1, 0);

  g(2, lx, 0) = g(1, lx, 0) - (2.0 / 3.0) * rho(lx, 0) * u(0, lx, 0);

  g(3, lx, 0) = g(4, lx, 0) + (2.0 / 3.0) * rho(lx, 0) * u(1, lx, 0);

  g(7, lx, 0) = g(8, lx, 0) - (1.0 / 6.0) * rho(lx, 0) * u(0, lx, 0) +
                (1.0 / 6.0) * rho(lx, 0) * u(1, lx, 0);

  g(5, lx, 0) = 0.0;
  g(6, lx, 0) = 0.0;

  g(0, lx, 0) = rho(lx, 0) - g(1, lx, 0) - g(2, lx, 0) - g(3, lx, 0) -
                g(4, lx, 0) - g(5, lx, 0) - g(6, lx, 0) - g(7, lx, 0) -
                g(8, lx, 0);
}

void zou_he_left_wall_velocity(Array3 &u, Array2x2d &u_left, Array2 &rho,
                               Array3 &g) {
  for (ll i = 0; i < rho.cols(); ++i) {
    u(0, 0, i) = u_left(0, i);
    u(1, 0, i) = u_left(1, i);

    rho(0, i) = (g(0, 0, i) + g(3, 0, i) + g(4, 0, i) + 2.0 * g(2, 0, i) +
                 2.0 * g(6, 0, i) + 2.0 * g(7, 0, i)) /
                (1.0 - u(0, 0, i));

    g(1, 0, i) = g(2, 0, i) + (2.0 / 3.0) * rho(0, i) * u(0, 0, i);

    g(5, 0, i) = g(6, 0, i) - (1.0 / 2.0) * (g(3, 0, i) - g(4, 0, i)) +
                 (1.0 / 6.0) * rho(0, i) * u(0, 0, i) +
                 (1.0 / 2.0) * rho(0, i) * u(1, 0, i);

    g(8, 0, i) = g(7, 0, i) + (1.0 / 2.0) * (g(3, 0, i) - g(4, 0, i)) +
                 (1.0 / 6.0) * rho(0, i) * u(0, 0, i) -
                 (1.0 / 2.0) * rho(0, i) * u(1, 0, i);
  }
}

void zou_he_right_wall_velocity(ll lx, Array3 &u, Array2x2d &u_right,
                                Array2 &rho, Array3 &g) {
  for (ll i = 0; i < rho.cols(); ++i) {
    u(0, lx, i) = u_right(0, i);
    u(1, lx, i) = u_right(1, i);

    rho(lx, i) = (g(0, lx, i) + g(3, lx, i) + g(4, lx, i) + 2.0 * g(1, lx, i) +
                  2.0 * g(5, lx, i) + 2.0 * g(8, lx, i)) /
                 (1.0 + u(0, lx, i));

    g(2, lx, i) = g(1, lx, i) - (2.0 / 3.0) * rho(lx, i) * u(0, lx, i);

    g(6, lx, i) = g(5, lx, i) + (1.0 / 2.0) * (g(3, lx, i) - g(4, lx, i)) -
                  (1.0 / 6.0) * rho(lx, i) * u(0, lx, i) -
                  (1.0 / 2.0) * rho(lx, i) * u(1, lx, i);

    g(7, lx, i) = g(8, lx, i) - (1.0 / 2.0) * (g(3, lx, i) - g(4, lx, i)) -
                  (1.0 / 6.0) * rho(lx, i) * u(0, lx, i) +
                  (1.0 / 2.0) * rho(lx, i) * u(1, lx, i);
  }
}

void zou_he_top_wall_velocity(ll ly, Array3 &u, Array2x2d &u_top, Array2 &rho,
                              Array3 &g) {
  for (ll i = 0; i < rho.rows(); ++i) {
    u(0, i, ly) = u_top(0, i);
    u(1, i, ly) = u_top(1, i);

    rho(i, ly) = (g(0, i, ly) + g(1, i, ly) + g(2, i, ly) + 2.0 * g(3, i, ly) +
                  2.0 * g(5, i, ly) + 2.0 * g(7, i, ly)) /
                 (1.0 + u(1, i, ly));

    g(4, i, ly) = g(3, i, ly) - (2.0 / 3.0) * rho(i, ly) * u(1, i, ly);

    g(8, i, ly) = g(7, i, ly) - (1.0 / 2.0) * (g(1, i, ly) - g(2, i, ly)) +
                  (1.0 / 2.0) * rho(i, ly) * u(0, i, ly) -
                  (1.0 / 6.0) * rho(i, ly) * u(1, i, ly);

    g(6, i, ly) = g(5, i, ly) + (1.0 / 2.0) * (g(1, i, ly) - g(2, i, ly)) -
                  (1.0 / 2.0) * rho(i, ly) * u(0, i, ly) -
                  (1.0 / 6.0) * rho(i, ly) * u(1, i, ly);
  }
}

void zou_he_bottom_wall_velocity(Array3 &u, Array2x2d &u_bot, Array2 &rho,
                                 Array3 &g) {
  for (ll i = 0; i < rho.rows(); ++i) {
    u(0, i, 0) = u_bot(0, i);
    u(1, i, 0) = u_bot(1, i);

    rho(i, 0) = (g(0, i, 0) + g(1, i, 0) + g(2, i, 0) + 2.0 * g(4, i, 0) +
                 2.0 * g(6, i, 0) + 2.0 * g(8, i, 0)) /
                (1.0 - u(1, i, 0));

    g(3, i, 0) = g(4, i, 0) + (2.0 / 3.0) * rho(i, 0) * u(1, i, 0);

    g(5, i, 0) = g(6, i, 0) - (1.0 / 2.0) * (g(1, i, 0) - g(2, i, 0)) +
                 (1.0 / 2.0) * rho(i, 0) * u(0, i, 0) +
                 (1.0 / 6.0) * rho(i, 0) * u(1, i, 0);

    g(7, i, 0) = g(8, i, 0) + (1.0 / 2.0) * (g(1, i, 0) - g(2, i, 0)) -
                 (1.0 / 2.0) * rho(i, 0) * u(0, i, 0) +
                 (1.0 / 6.0) * rho(i, 0) * u(1, i, 0);
  }
}

void save_velocity(ll it, const Array3 &u) {
  for(ll dim = 0; dim < 2; ++dim) {
    char buffer[20];
    std::sprintf(buffer, "u.%.6lld.%lld.txt", it, dim);
    std::ofstream fout(buffer);
    fout << std::scientific << std::setprecision(17);
    for(ll i = 0; i < u.dimension(1); ++i) {
      for(ll j = 0; j < u.dimension(2); ++j) {
        fout << u(dim, i, j) << " ";
      }
      fout << "\n";
    }
  }
}


int main() {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();

  const double re_lbm = 100.0;

  const double u_lbm = 0.2;
  const double rho_lbm = 1.0;
  const double t_max = 20.0;
  const double x_min = 0.0;
  const double x_max = 1.0;
  const double y_min = 0.0;
  const double y_max = 1.0;
  const double c_s = 1.0 / std::sqrt(3.0);

  const ll npts = 100;
  const ll ny = npts;
  const double nu_lbm = u_lbm * npts / re_lbm;
  const double tau_lbm = 0.5 + nu_lbm / (c_s * c_s);
  const double dt = re_lbm * nu_lbm / (npts * npts);
  //const double dy = (y_max - y_min) / ny;
  //const double dx = dy;
  const ll nx = (ll)std::round(ny * (x_max - x_min) / (y_max - y_min));

  const ll it_max = (ll)std::round(t_max / dt);
  const double sigma = 10.0 * nx;

  const ll lx = nx - 1;
  const ll ly = ny - 1;

  const double tau_p_lbm = tau_lbm;
  const double lambda_trt = 1.0 / 4.0;  // Best for stability
  const double tau_m_lbm = lambda_trt / (tau_p_lbm - 0.5) + 0.5;
  const double om_p_lbm = 1.0 / tau_p_lbm;
  const double om_m_lbm = 1.0 / tau_m_lbm;
  // const double om_lbm = 1.0 / tau_lbm;

  // # Density arrays
  Array3 g(Q, nx, ny);
  g.setZero();
  Array3 g_eq(Q, nx, ny);
  g_eq.setZero();
  Array3 g_up(Q, nx, ny);
  g_up.setZero();

  // # Boundary conditions
  Array2x2d u_left = Array2x2d::Zero(2, ny);
  Array2x2d u_right = Array2x2d::Zero(2, ny);
  Array2x2d u_top = Array2x2d::Zero(2, nx);
  Array2x2d u_bot = Array2x2d::Zero(2, nx);

  // # Physical fields
  Array2 rho = Array2::Ones(nx, ny);
  Array3 u(2, nx, ny);
  u.setZero();

  // Initialize and compute first equilibrium
  set_inlets(sigma, u_lbm, 0, u_top, u_bot, u_left, u_right);
  rho *= rho_lbm;
  compute_equilibrium(u, rho, g_eq);
  g = g_eq;

  std::unordered_map<std::string, ll> profiler;
  profiler["compute_equilibrium"] = 0;
  profiler["set_inlets"] = 0;
  profiler["compute_macroscopic"] = 0;
  profiler["collision_and_streaming"] = 0;
  profiler["boundary_conditions"] = 0;

  for (ll it = 0; it < it_max + 1; ++it) {
    if (it % 100 == 0) {
      std::cout << "Iteration: " << it << " / " << it_max << std::endl;
      save_velocity(it, u);
    }
    // 1. Set inlets
    auto tt = high_resolution_clock::now();
    set_inlets(sigma, u_lbm, it, u_top, u_bot, u_left, u_right);
    auto tte = high_resolution_clock::now();
    auto dtt = duration_cast<nanoseconds>(tte - tt).count();
    profiler["set_inlets"] += dtt;
    // 2. Compute macroscopic fields
    tt = high_resolution_clock::now();
    compute_macroscopic(g, u, rho);
    tte = high_resolution_clock::now();
    dtt = duration_cast<nanoseconds>(tte - tt).count();
    profiler["compute_macroscopic"] += dtt;
    // TBD. Output field
    // 4. Compute equilibrium state
    tt = high_resolution_clock::now();
    compute_equilibrium(u, rho, g_eq);
    tte = high_resolution_clock::now();
    dtt = duration_cast<nanoseconds>(tte - tt).count();
    profiler["compute_equilibrium"] += dtt;
    // 5. Streaming
    tt = high_resolution_clock::now();
    collision_and_streaming(om_p_lbm, om_m_lbm, lx, ly, g_eq, g, g_up);
    tte = high_resolution_clock::now();
    dtt = duration_cast<nanoseconds>(tte - tt).count();
    profiler["collision_and_streaming"] += dtt;
    // 6. Boundary conditions
    tt = high_resolution_clock::now();
    zou_he_bottom_wall_velocity(u, u_bot, rho, g);
    zou_he_left_wall_velocity(u, u_left, rho, g);
    zou_he_right_wall_velocity(lx, u, u_right, rho, g);
    zou_he_top_wall_velocity(ly, u, u_top, rho, g);
    zou_he_bottom_left_corner_velocity(u, rho, g);
    zou_he_top_left_corner_velocity(ly, u, rho, g);
    zou_he_top_right_corner_velocity(lx, ly, u, rho, g);
    zou_he_bottom_right_corner_velocity(lx, u, rho, g);
    tte = high_resolution_clock::now();
    dtt = duration_cast<nanoseconds>(tte - tt).count();
    profiler["boundary_conditions"] += dtt;
    // TBD: Compute observables (drag, lift, etc)
  }

  const auto t1 = high_resolution_clock::now();
  const auto elapsed = duration_cast<milliseconds>(t1 - t0).count();
  std::cout << "Elapsed: " << elapsed << std::endl;
  for (const auto &[key, value] : profiler) {
    std::cout << key << ": " << value / 1000000 << std::endl;
  }


// {
//   std::vector<double> tx(nx), vx(ny), uy(nx);
//   for(size_t i = 0; i < vx.size(); ++i)
//     vx[i] = u(1,i,ny/2)/u_lbm;
//   for(size_t j = 0; j < uy.size(); ++j) {
//     uy[j] = u(0,nx/2,j)/u_lbm;
//     tx[j] = double(j) / (nx - 1);
//   }

//   Gnuplot gp;
//   //gp << "set xrange [0:1]\nset yrange [-1:1]\n";
    
//   gp << "plot '-' with lines title 'uy', '-' with lines title 'vx'\n";
//   gp.send1d(std::make_tuple(tx, uy));
//   gp.send1d(std::make_tuple(tx, vx));
// }
   




  return 0;
}
