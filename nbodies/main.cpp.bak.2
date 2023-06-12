#include <iostream>
#include <vector>
#include <chrono>
#include <array>
#include <memory>
#include <cassert>

#include <Eigen/Dense>

// ============================================================================================

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

    all_children->push_back({});
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
      m_tree.corners(j, 0) = std::numeric_limits<T>::max();
      m_tree.corners(j, 1) = -std::numeric_limits<T>::max();
    }
    for(const auto &p : m_particles) {
      for(int j = 0; j < dim; ++j) {
        constexpr T eps = 2.0 * std::numeric_limits<T>::epsilon();
        m_tree.corners(j, 0) = std::min(m_tree.corners(j, 0), p.pos[j]);
        m_tree.corners(j, 1) = std::max(m_tree.corners(j, 1), p.pos[j] + eps);
      }
    }
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

// ============================================================================================

constexpr double G = 1.0;
constexpr double softening = 1e-2;
constexpr double theta = 0.5;

template<typename T, int dim>
inline Vect<T, dim> compute_force(const Vect<T, dim> &a, const Vect<T, dim> &b) {
  const Vect<T, dim> r = a - b;
  const T inv_r3 = std::pow((r * r).sum() + softening, -1.5);
  return r * (G * inv_r3);
}

template<typename T, int dim>
void update_acceleration(std::vector<Particle<T, dim>> &particles) {
  for(auto &p : particles)
    for(size_t i = 0; i < dim; ++i)
      p.acc[i] = 0.0;
 
  for(size_t i = 0; i < particles.size(); ++i) {
    for(size_t j = 0; j < i; ++j) {
      const auto force = compute_force(particles[i].pos, particles[j].pos);
      particles[i].acc += force * particles[j].mass;
      particles[j].acc += force * particles[i].mass;
    }
  }
}

template<typename T, int dim>
void update_acceleration(Tree<T, dim> &tree) {
  auto &particles = tree.get_particles();
  for(auto &p : particles)
    for(size_t i = 0; i < dim; ++i)
      p.acc[i] = 0.0;
 
  int32_t n_particles = particles.size();
  for(int32_t i = 0; i < n_particles; ++i) {
    const auto f = [&](const TreeNode<T, dim> &node){
      //std::cout << &node << std::endl;
      if(node.children_idx != -1) {
        const auto &node_data = tree.get_nodes()[node.children_idx].data;
        const Vect<T, dim> center_of_mass = node_data.mass_times_position / node_data.mass;
        const auto d = norm<dim>(center_of_mass, particles[i].pos);
        if(node_data.h / d < theta) {
          const auto force = compute_force(particles[i].pos, center_of_mass);
          particles[i].acc += force * node_data.mass;
          return true;
        } else {
          return false;
        }
      } else {
        if(i == node.particle_idx)
          return true;
        if(node.particle_idx != -1) {
          const auto force = compute_force(particles[i].pos, particles[node.particle_idx].pos);
          particles[i].acc += force * particles[node.particle_idx].mass;
        }
        return true;
      }
    };
    tree.traverse_dfs(f);
    //tree.traverse_bfs(f);
  }
}

template<typename T, int dim>
void integrate(std::vector<Particle<T, dim>> &particles, double dt) {
  for(auto &p : particles) {
    p.pos += p.vel * dt;
    p.vel += p.acc * dt;
  }
}


template<typename T, int dim>
std::vector<Particle<T, dim>> initialize(size_t n) {
  using P = Particle<T, dim>;
  using Vec = Vect<T, dim>;

  T total_mass = 0.0;
  Vec total_momentum = Vec::Zero();
  std::vector<P> particles;
  for(size_t i = 0; i < n; ++i){
    particles.push_back({1.0, Vec::Random(), Vec::Random(), Vec::Zero()});
    total_mass += particles.back().mass;
    total_momentum += particles.back().vel * particles.back().mass;
  }
  
  const auto center_of_mass = total_momentum / total_mass;
  for(auto &p : particles)
    p.vel -= center_of_mass;
  return particles;
}


int main(int argc, char **argv) {
  using real = double;
  constexpr int dim = 3;

  if(argc < 2)
    return -1;

  double t_fin = 0.1;
  double dt = 1e-2;
  size_t n = std::atoll(argv[1]);

  using namespace std::chrono;

  {
    auto particles = initialize<real, dim>(n);
    const auto t0 = high_resolution_clock::now();
    for(double t = 0.0; t < t_fin; t += dt) {
      std::cout << "t:" << t << std::endl;
      update_acceleration<real, dim>(particles);
      integrate<real, dim>(particles, dt);
    }
    const auto t1 = high_resolution_clock::now();
    std::cout << "Elapsed: " << duration_cast<milliseconds>(t1 - t0).count() << std::endl;
  }

  {
    std::cout << "sizeof node: " << sizeof(TreeNode<real, dim>) << std::endl;
    std::cout << "sizeof chil: " << sizeof(typename TreeNode<real, dim>::ChildrenType) << std::endl;

    size_t t_tree = 0, t_update = 0, t_integration = 0;

    auto particles = initialize<real, dim>(n);
    
    for(double t = 0.0; t < t_fin; t += dt) {
      std::cout << "t:" << t << std::endl;

      auto t0 = high_resolution_clock::now();
      auto tree = Tree<real, dim>(particles);
      auto t1 = high_resolution_clock::now();
      t_tree += duration_cast<milliseconds>(t1 - t0).count();

      // ========================================================================
      // reordering particles in bfs order
      // ========================================================================
      std::vector<int32_t> particles_po;
      std::vector<Particle<real, dim>> new_particles;
      // check and save the bfs order
      tree.traverse_bfs([&](const TreeNode<real, dim> &node){ 
        if(node.children_idx != -1) {
          return false;
        } else {
          if(node.particle_idx != -1)
            particles_po.push_back(node.particle_idx);
          return true;
        }
      });
      // make copy of new order
      for(const auto i : particles_po)
        new_particles.push_back(particles[i]);
      // apply new order to tree
      int32_t count = 0;
      tree.traverse_bfs([&](TreeNode<real, dim> &node){ 
        if(node.children_idx != -1) {
          return false;
        } else {
          if(node.particle_idx != -1)
            node.particle_idx = count++;
          return true;
        }
      });
      tree.set_particles(&new_particles);
      
      // ========================================================================
      // reordering nodes in bfs order
      // ========================================================================
      std::vector<TreeNode<real, dim> *> curr = {&tree.get_root()}, next;
      std::vector<int32_t> nodes;
      std::vector<typename TreeNode<real, dim>::ChildrenType> tmp;
      std::vector<std::array<int32_t, (1ull << dim)>> tmp2;
      // check and save the bfs order
      while(!curr.empty()) {
        for(auto &node : curr) {
          if(node->children_idx != -1) {
            nodes.push_back(node->children_idx);
            for(size_t i = 0; i < (1ull << dim); ++i) {
              next.push_back(&(tree.get_nodes()[node->children_idx].children[i]));
            }
          }
        }
        std::swap(curr, next);
        next.clear();
      }
      // make new order (for each node find the children idxs)
      for(auto np : nodes) {
        tmp.push_back(tree.get_nodes()[np]);
        tmp2.push_back({});
        for(size_t j = 0; j < (1ull << dim); ++j) {
          for(int32_t i = 0; i < int32_t(nodes.size()); ++i) {
            if( tree.get_nodes()[np].children[j].children_idx == i ) {
              tmp2.back()[j] = i;
              //std::cout << "=" << i << std::endl;
              break;
            }
          }
        }
      }
      // set new order
      tree.set_nodes(tmp);
      for(size_t i = 0; i < nodes.size(); ++i) {
        for(size_t j = 0; j < (1ull << dim); ++j)
        if(tmp2[i][j] != 0)
          tree.get_nodes()[i].children[j].children_idx = tmp2[i][j];
        else 
          tree.get_nodes()[i].children[j].children_idx = -1;
      }
      // ========================================================================

      // TODO: use same trick to reorder node data 

      t0 = high_resolution_clock::now();
      update_acceleration<real, dim>(tree);
      t1 = high_resolution_clock::now();
      t_update += duration_cast<milliseconds>(t1 - t0).count();
      std::cout << "nnodes" << tree.nnodes() << std::endl;

      t0 = high_resolution_clock::now();
      integrate<real, dim>(particles, dt);
      t1 = high_resolution_clock::now();
      t_integration += duration_cast<milliseconds>(t1 - t0).count();
    }
    
    std::cout << "Elapsed: " << t_tree + t_update + t_integration << std::endl;
    std::cout << " - tree: " << t_tree << std::endl;
    std::cout << " - update: " << t_update << std::endl;
    std::cout << " - integration: " << t_integration << std::endl;
  }


  return 0;
}

