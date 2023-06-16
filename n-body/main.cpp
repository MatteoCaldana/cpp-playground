#include <iostream>
#include <vector>
#include <chrono>
#include <array>
#include <memory>
#include <cassert>

#include "quadtree.hpp"
#include "nbody.hpp"
#include "plot.hpp"

int main(int argc, char **argv) {
  using real = double;
  constexpr int dim = 2;
  constexpr TraverseOrder order = TraverseOrder::DFS;

  if(argc < 2)
    return -1;

  double t_fin = 1.0;
  double dt = 1e-2;
  size_t n = std::atoll(argv[1]);

  using namespace std::chrono;

  // {
  //   Gnuplot gp;
  //   gp << "set xrange [-1:1]\nset yrange [-1:1]\nset multiplot\n";
    
  //   auto particles = initialize<real, dim>(n);
  //   const auto t0 = high_resolution_clock::now();
  //   for(double t = 0.0; t < t_fin; t += dt) {
  //     std::cout << "t:" << t << std::endl;
  //     plot_particles(gp, particles);
  //     update_acceleration<real, dim>(particles);
  //     integrate<real, dim>(particles, dt);
  //   }
  //   const auto t1 = high_resolution_clock::now();
  //   std::cout << "Elapsed: " << duration_cast<milliseconds>(t1 - t0).count() << std::endl;
  // }

  {
    std::cout << "sizeof node: " << sizeof(TreeNode<real, dim>) << std::endl;
    std::cout << "sizeof chil: " << sizeof(typename TreeNode<real, dim>::ChildrenType) << std::endl;

    size_t t_tree = 0, t_update = 0, t_integration = 0, t_compress = 0;

    Gnuplot gp;
    gp << "set xrange [-1:1]\nset yrange [-1:1]\nset multiplot\n";

    auto particles = initialize<real, dim>(n);

    for(double t = 0.0; t < t_fin; t += dt) {
      std::cout << "t:" << t << std::endl;

      std::cout << "Building tree"<<std::endl;
      auto t0 = high_resolution_clock::now();
      auto tree = Tree<real, dim>(particles);
      auto t1 = high_resolution_clock::now();
      t_tree += duration_cast<microseconds>(t1 - t0).count();

      std::cout << "Compress tree"<<std::endl;
      t0 = high_resolution_clock::now();
      // TODO: check this works properly, code becomes too fast
      tree.compress<order>();
      t1 = high_resolution_clock::now();
      t_compress += duration_cast<microseconds>(t1 - t0).count();

      std::cout << "Integrate tree"<<std::endl;
      t0 = high_resolution_clock::now();
      update_acceleration<real, dim, order>(tree);
      t1 = high_resolution_clock::now();
      t_update += duration_cast<microseconds>(t1 - t0).count();

      std::cout << "Building tree"<<std::endl;
      t0 = high_resolution_clock::now();
      integrate<real, dim>(particles, dt);
      t1 = high_resolution_clock::now();
      t_integration += duration_cast<microseconds>(t1 - t0).count();

      std::cout << tree.depth() << std::endl;

      //plot_particles(gp, particles);
    }
    
    std::cout << "Elapsed: " << (t_tree + t_update + t_compress + t_integration) / 1000 << std::endl;
    std::cout << " - tree:        " << t_tree / 1000 << std::endl;
    std::cout << " - compress:    " << t_compress / 1000 << std::endl;
    std::cout << " - update:      " << t_update / 1000 << std::endl;
    std::cout << " - integration: " << t_integration / 1000 << std::endl;
  }

  return 0;
}

