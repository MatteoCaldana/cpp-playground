#pragma once

#include "gnuplot-iostream.h"
#include "quadtree.hpp"

template<typename T, int dim>
void plot_particles(Gnuplot &gp, const std::vector<Particle<T, dim>> &particles) {
    std::vector<double> x, y;
    for(const auto &p : particles) {
      x.push_back(p.pos[0]);
      y.push_back(p.pos[1]);
    }
    gp << "plot '-' title ''\n";
    gp.send1d(std::make_tuple(x, y));
}

template<typename T, int dim>
void plot_tree(Gnuplot &gp, const Tree<T, dim> &tree) {
  tree.traverse_bfs([&](const auto &node){
    gp << "plot '-' with lines title ''\n";
    const std::vector<double> x = {node.corners(0, 0), node.corners(0, 1), node.corners(0, 1), node.corners(0, 0), node.corners(0, 0)};
    const std::vector<double> y = {node.corners(1, 0), node.corners(1, 0), node.corners(1, 1), node.corners(1, 1), node.corners(1, 0),};
    gp.send1d(std::make_tuple(x, y));
    if(node.children_idx == -1)
      return true;
    return false;
  });
}
