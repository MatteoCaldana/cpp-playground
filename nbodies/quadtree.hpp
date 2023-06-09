#pragma once

#include "traits.hpp"

template<typename T, int dim>
struct NodeData {
  using Vec = Vect<T, dim>;

  T mass = 0.0;
  T h = -1.0;
  Vec mass_times_position = Vec::Zero();
};

template<typename T, int dim>
struct TreeNode {
  using P = Particle<T, dim>;
  using Vec = Vect<T, dim>;
  static constexpr inline size_t n_children = (1ull << dim);

  struct ChildrenType {
    std::array<TreeNode, n_children> children;
    NodeData<T, dim> data;
  };
  
  // TODO: use a lower precision type for corners?
  //       tradeoff cast time vs space
  Eigen::Array<int32_t, dim, 1> corners;
  int32_t depth = -1;
  int32_t children_idx = -1;
  int32_t particle_idx = -1;

  bool inside(const Vec &position) {
    const auto h = 2.0 / (1 << depth);
    for(int i = 0; i < dim; ++i)
      if(position[i] < h * corners(i, 0) - 1.0 || h * (1 + corners(i, 0)) - 1.0 <= position[i]) {
        return false;
      }
    return true;
  }

  bool insert(
    const Vec &position, 
    int32_t new_particle_idx, 
    P *all_particles, 
    std::vector<ChildrenType> *all_children
  ) {
    if(!inside(position)){
      return false;
    }

    if(particle_idx == -1) {
      particle_idx = new_particle_idx;
    } else {
      if(children_idx == -1) {
        children_idx = create_children(all_children);
        const auto particle = all_particles + particle_idx;

        auto &node_data = (*all_children)[children_idx].data;
        node_data.h = 2.0 * 1.141 / (1 << depth);
        node_data.mass = particle->mass;
        node_data.mass_times_position += particle->mass * particle->pos;

        for(size_t i = 0; i < n_children; ++i)
          (*all_children)[children_idx].children[i].insert(particle->pos, particle_idx, all_particles, all_children);
      }
      for(size_t i = 0; i < n_children; ++i)
        (*all_children)[children_idx].children[i].insert(position, new_particle_idx, all_particles, all_children);

      // update CenterOfMass
      const auto new_particle = all_particles + new_particle_idx;
      auto &node_data = (*all_children)[children_idx].data;
      node_data.mass += new_particle->mass;
      node_data.mass_times_position += new_particle->mass * new_particle->pos;
    }

    return true;
  };

  int32_t create_children(std::vector<ChildrenType> *all_children) {
    all_children->push_back({});
    assert((*all_children).size() > 0);
    int32_t new_children_idx = (*all_children).size() - 1;
    for(size_t i = 0; i < n_children; ++i) {
      for(int j = 0; j < dim; ++j) {
        const auto idx = (i >> j) % 2;
        (*all_children)[new_children_idx].children[i].corners(j, 0) = 2 * corners(j, 0) + idx;
      }
      (*all_children)[new_children_idx].children[i].depth = depth + 1;
    }
    return new_children_idx;
  }

  void traverse(const std::function<bool(const TreeNode<T, dim> &)> &f, std::vector<ChildrenType> *all_children) const {
    if(f(*this))
      return;
    if(children_idx != -1) {
      for(auto &p : (*all_children)[children_idx].children){
        p.traverse(f, all_children);
      }
    }
  }

  size_t nnodes(std::vector<ChildrenType> *all_children) const {
    size_t nnodes_children = 0;
    if(children_idx != -1) {
      for(const auto &c : (*all_children)[children_idx].children)
        nnodes_children += c.nnodes(all_children);
    }
    return 1 + nnodes_children;
  }
};

template<typename T, int dim>
class Tree {
  using P = Particle<T, dim>;
  using Vec = Vect<T, dim>;
  using ChildrenType = typename TreeNode<T, dim>::ChildrenType;

public:
  Tree(std::vector<P> &particles) : m_particles(particles), m_nodes(0) {
    m_nodes.reserve(m_particles.size() * (1ull << dim));

    for(int j = 0; j < dim; ++j) {
      m_tree.corners(j, 0) = 0;
    }
    m_tree.depth = 0;
    for(size_t i = 0; i < m_particles.size(); ++i) {
      m_tree.insert(m_particles[i].pos, i, m_particles.data(), &m_nodes);
    }
  }

  std::vector<P> &get_particles() {
    return m_particles;
  }

  const TreeNode<T, dim> &get_root() const {
    return m_tree;
  }

  TreeNode<T, dim> &get_root() {
    return m_tree;
  }

  std::vector<ChildrenType> &get_nodes() {
    return m_nodes;
  }

  void set_nodes(const std::vector<ChildrenType> & nodes) {
    m_nodes = nodes;
  }

  void set_particles(std::vector<P> *paticles) {
    m_particles = (*paticles);
  }

  size_t nnodes() {
    return m_tree.nnodes(&m_nodes);
  }

  void traverse_bfs(const std::function<bool(TreeNode<T, dim> &)> &f) {
    std::vector<TreeNode<T, dim> *> curr = {&m_tree}, next;
    while(!curr.empty()) {
      for(auto &node : curr) {
        if(!f(*node)) {
          for(size_t i = 0; i < TreeNode<T, dim>::n_children; ++i) {
            next.push_back(&(m_nodes[node->children_idx].children[i]));
          }
        }
      }
      std::swap(curr, next);
      next.clear();
    }
  }

  void traverse_dfs(const std::function<bool(const TreeNode<T, dim> &)> &f) {
    m_tree.traverse(f, &m_nodes);
  }

private:
  std::vector<P> &m_particles;
  std::vector<ChildrenType> m_nodes;
  TreeNode<T, dim> m_tree;
};
