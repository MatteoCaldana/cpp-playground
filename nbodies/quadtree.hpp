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

  // TODO: put node data in another vector
  struct ChildrenType {
    std::array<TreeNode, n_children> children;
    NodeData<T, dim> data;
  };
  
  // TODO: use cartesian index to indicate cell?
  Eigen::Array<T, dim, 2> corners;
  int32_t children_idx = -1;
  int32_t particle_idx = -1;

  bool inside(const Vec &position) {
    for(int i = 0; i < dim; ++i)
      if(position[i] < corners(i, 0) || corners(i, 1) <= position[i]) {
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
        node_data.h = norm<dim>(corners.col(0), corners.col(1));
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
    Eigen::Array<T, dim, 3> corners_ext;
    corners_ext.col(0) = corners.col(0);
    corners_ext.col(2) = corners.col(1);
    corners_ext.col(1) = (corners.col(0) + corners.col(1)) / 2;

    all_children->emplace_back();
    assert((*all_children).size() > 0);
    int32_t new_children_idx = (*all_children).size() - 1;
    for(size_t i = 0; i < n_children; ++i) {
      for(int j = 0; j < dim; ++j) {
        const auto idx = (i >> j) % 2;
        (*all_children)[new_children_idx].children[i].corners(j, 0) = corners_ext(j, idx);
        (*all_children)[new_children_idx].children[i].corners(j, 1) = corners_ext(j, idx + 1);
      }
    }
    return new_children_idx;
  }

  void traverse(const std::function<bool(TreeNode<T, dim> &)> &f, std::vector<ChildrenType> *all_children) {
    if(f(*this))
      return;
    assert(children_idx != -1);
    for(auto &p : (*all_children)[children_idx].children){
      p.traverse(f, all_children);
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

enum class TraverseOrder {
  BFS = 0,
  DFS = 1
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
      m_tree.corners(j, 0) = std::numeric_limits<T>::max();
      m_tree.corners(j, 1) = -std::numeric_limits<T>::max();
    }
    for(const auto &p : m_particles) {
      for(int j = 0; j < dim; ++j) {
        const auto eps = std::abs(p.pos[j]) * (2.0 * std::numeric_limits<T>::epsilon());
        m_tree.corners(j, 0) = std::min(m_tree.corners(j, 0), p.pos[j]);
        m_tree.corners(j, 1) = std::max(m_tree.corners(j, 1), p.pos[j] + eps);
      }
    }
    for(size_t i = 0; i < m_particles.size(); ++i) {
      if(!m_tree.insert(m_particles[i].pos, i, m_particles.data(), &m_nodes)) {
        std::cout << "ERROR: could not insert particle in tree" << std::endl;
        std::cout << m_tree.corners << std::endl;
        std::cout << m_particles[i].pos << std::endl;
      }
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

  void traverse_dfs(const std::function<bool(TreeNode<T, dim> &)> &f) {
    m_tree.traverse(f, &m_nodes);
  }

  template<TraverseOrder O>
  void traverse(const std::function<bool(TreeNode<T, dim> &)> &f) {
    if constexpr (O == TraverseOrder::BFS)
      traverse_bfs(f);
    else if constexpr (O == TraverseOrder::DFS)
      traverse_dfs(f);
    else 
      std::exit(-1);
  }

  // re-order underlying bufferns in order to be cache friendly
  template<TraverseOrder O>
  void compress() {
    std::vector<int32_t> particle_idx_table(m_particles.size());
    std::vector<int32_t> node_idx_table(m_nodes.size());

    int32_t particle_idx = 0;
    int32_t node_idx = 0;

    traverse<O>([&](TreeNode<T, dim> &node) {
      if(node.children_idx != -1) {
        node_idx_table[node_idx] = node.children_idx;
        node.children_idx = node_idx;
        node_idx++;
        return false;
      } else if (node.particle_idx != -1) {
        particle_idx_table[particle_idx] = node.particle_idx;
        node.particle_idx = particle_idx;
        particle_idx++;
      }
      return true;
    });

    const std::vector tmp_particles = m_particles;
    for(size_t i = 0; i < particle_idx_table.size(); ++i) {
      m_particles[i] = tmp_particles[particle_idx_table[i]];
    }

    const std::vector tmp_nodes = m_nodes;
    std::vector<int32_t> node_idx_table_inverse(m_nodes.size());
    
    for(size_t i = 0; i < node_idx_table.size(); ++i) {
      m_nodes[i] = tmp_nodes[node_idx_table[i]];
      node_idx_table_inverse[node_idx_table[i]] = i;
    }

    m_tree.children_idx = node_idx_table_inverse[m_tree.children_idx];
    for(size_t i = 0; i < node_idx_table.size(); ++i) {
      for(size_t j = 0; j < TreeNode<T, dim>::n_children; ++j) {
        if(m_nodes[i].children[j].children_idx != -1) {
          m_nodes[i].children[j].children_idx = node_idx_table_inverse[m_nodes[i].children[j].children_idx];
        }
      }
    }

  }

private:
  std::vector<P> &m_particles;
  std::vector<ChildrenType> m_nodes;
  TreeNode<T, dim> m_tree;
};
