#pragma once

#include "traits.hpp"

constexpr double G = 1.0;
constexpr double softening = 1e-2;
constexpr double theta = 0.5;

template<typename T, int dim>
inline Vect<T, dim> compute_force(const Vect<T, dim> &a, const Vect<T, dim> &b) {
  const Vect<T, dim> r = a - b;
  T norm2 = softening;
  for(int i = 0; i < dim; ++i)
    norm2 += r[i] * r[i];
  return -r * (G * std::pow(norm2, -1.5));
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
      particles[j].acc -= force * particles[i].mass;
    }
  }
}

template<typename T, int dim, TraverseOrder O>
void update_acceleration(Tree<T, dim> &tree) {
  auto &particles = tree.get_particles();
  for(auto &p : particles)
    for(size_t i = 0; i < dim; ++i)
      p.acc[i] = 0.0;
 
  const int32_t n_particles = particles.size();
  for(int32_t i = 0; i < n_particles; ++i) {
    const auto f = [&](const TreeNode<T, dim> &node){
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
    tree.template traverse<O>(f);
  }
}

template<typename T, int dim>
void integrate(std::vector<Particle<T, dim>> &particles, double dt) {
  for(auto &p : particles) {
    p.vel += p.acc * dt;
    p.pos += p.vel * dt;
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