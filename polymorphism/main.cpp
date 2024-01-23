#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <variant>
#include <vector>

// Base class template using CRTP
template <typename Derived>
class Shape {
 public:
  void draw() { static_cast<Derived *>(this)->drawImpl(); }
  void count() { static_cast<Derived *>(this)->countImpl(); }
  size_t counter = 0;
};

class Circle : public Shape<Circle> {
 public:
  void drawImpl() { std::cout << "Drawing Circle\n"; }
  void countImpl() { counter++; }
};

class Rectangle : public Shape<Rectangle> {
 public:
  void drawImpl() { std::cout << "Drawing Rectangle\n"; }
  void countImpl() { counter += 2; }
};

class Triangle : public Shape<Triangle> {
 public:
  void drawImpl() { std::cout << "Drawing Triangle\n"; }
  void countImpl() { counter += 3; }
};

using ShapeVar = std::variant<Circle, Rectangle, Triangle>;
using ShapeVarPtr =
    std::variant<std::unique_ptr<Circle>, std::unique_ptr<Rectangle>,
                 std::unique_ptr<Triangle>>;

ShapeVar shape_factory(int id) {
  if (id == 1)
    return Circle{};
  else if (id == 2)
    return Rectangle{};
  else if (id == 3)
    return Triangle{};
  else {
    std::cerr << "Unknown shape id" << std::endl;
    std::exit(-1);
  }
}

ShapeVarPtr shape_factory_ptr(int id) {
  if (id == 1)
    return std::make_unique<Circle>();
  else if (id == 2)
    return std::make_unique<Rectangle>();
  else if (id == 3)
    return std::make_unique<Triangle>();
  else {
    std::cerr << "Unknown shape id" << std::endl;
    std::exit(-1);
  }
}

// Base class template using Polymorphism
class AbstractShape {
 public:
  virtual void draw() const = 0;
  virtual void count() = 0;
  size_t counter = 0;
};

class VirtualCircle : public AbstractShape {
 public:
  virtual void draw() const override { std::cout << "Drawing Circle\n"; };
  virtual void count() override { counter++; };
};

class VirtualRectangle : public AbstractShape {
 public:
  virtual void draw() const override { std::cout << "Drawing Rectangle\n"; };
  virtual void count() override { counter += 2; };
};

class VirtualTriangle : public AbstractShape {
 public:
  virtual void draw() const override { std::cout << "Drawing Triangle\n"; };
  virtual void count() override { counter += 3; };
};

std::unique_ptr<AbstractShape> virtual_shape_factory(int id) {
  if (id == 1)
    return std::make_unique<VirtualCircle>();
  else if (id == 2)
    return std::make_unique<VirtualRectangle>();
  else if (id == 3)
    return std::make_unique<VirtualTriangle>();
  else {
    std::cerr << "Unknown shape id" << std::endl;
    std::exit(-1);
  }
}
using namespace std::chrono;
int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Not enough arguments, need: vector_size, repetitions"
              << std::endl;
    std::exit(-1);
  }
  auto vector_size = std::atoll(argv[1]);
  auto repetitions = std::atoll(argv[2]);

  std::cout << "vector_size:" << vector_size << std::endl;
  std::cout << "repetitions:" << repetitions << std::endl;

  std::vector<ShapeVar> shapes;
  std::vector<ShapeVarPtr> shapes_ptrs;
  std::vector<std::unique_ptr<AbstractShape>> vitual_shapes;

  {
    std::default_random_engine engine(0);
    std::uniform_int_distribution<int> uniform_dist(1, 3);
    auto t0 = high_resolution_clock::now();
    for (auto i = 0ll; i < vector_size; ++i) {
      shapes.push_back(shape_factory(uniform_dist(engine)));
    }
    auto t1 = high_resolution_clock::now();
    auto dt = duration_cast<microseconds>(t1 - t0).count();
    std::cout << "Allocation CRTP elapsed:    " << dt / 1000.0 << "[ms]"
              << std::endl;
  }

  {
    std::default_random_engine engine(0);
    std::uniform_int_distribution<int> uniform_dist(1, 3);
    auto t0 = high_resolution_clock::now();
    for (auto i = 0ll; i < vector_size; ++i) {
      shapes_ptrs.push_back(shape_factory_ptr(uniform_dist(engine)));
    }
    auto t1 = high_resolution_clock::now();
    auto dt = duration_cast<microseconds>(t1 - t0).count();
    std::cout << "Allocation CRTP ptr elapsed:" << dt / 1000.0 << "[ms]"
              << std::endl;
  }

  {
    std::default_random_engine engine(0);
    std::uniform_int_distribution<int> uniform_dist(1, 3);
    auto t0 = high_resolution_clock::now();
    for (auto i = 0ll; i < vector_size; ++i) {
      vitual_shapes.push_back(virtual_shape_factory(uniform_dist(engine)));
    }
    auto t1 = high_resolution_clock::now();
    auto dt = duration_cast<microseconds>(t1 - t0).count();
    std::cout << "Allocation virtual elapsed: " << dt / 1000.0 << "[ms]"
              << std::endl;
  }

  {
    auto t0 = high_resolution_clock::now();
    for (auto i = 0ll; i < repetitions; ++i) {
      for (auto &shape : shapes) {
        std::visit([](auto &&s) { s.count(); }, shape);
      }
    }
    auto t1 = high_resolution_clock::now();
    auto dt = duration_cast<microseconds>(t1 - t0).count();
    std::cout << "Count CRTP elapsed:         " << dt / 1000.0 << "[ms]" << std::endl;
  }

  {
    auto t0 = high_resolution_clock::now();
    for (auto i = 0ll; i < repetitions; ++i) {
      for (const auto &shape : shapes_ptrs) {
        std::visit([](auto &&s) { s->count(); }, shape);
      }
    }
    auto t1 = high_resolution_clock::now();
    auto dt = duration_cast<microseconds>(t1 - t0).count();
    std::cout << "Count CRTP ptr elapsed:     " << dt / 1000.0 << "[ms]" << std::endl;
  }
  {
    auto t0 = high_resolution_clock::now();
    for (auto i = 0ll; i < repetitions; ++i) {
      for (const auto &shape : vitual_shapes) {
        shape->count();
      }
    }
    auto t1 = high_resolution_clock::now();
    auto dt = duration_cast<microseconds>(t1 - t0).count();
    std::cout << "Count virtual elapsed:      " << dt / 1000.0 << "[ms]" << std::endl;
  }

  return 0;
}
