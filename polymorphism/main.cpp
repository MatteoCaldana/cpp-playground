#include <iostream>
#include <variant>
#include <vector>
#include <random>
#include <memory>
#include <chrono>

struct Shape {
  virtual void greet() const = 0;
  virtual double area() const = 0;
  virtual ~Shape() = default;
};

struct Square : public Shape {
  virtual void greet() const override { std::cout << "Square\n"; } ;
  virtual double area() const override { return side * side; };
  virtual ~Square() override = default;
  double side;
};

struct Rectangle : public Shape {
  virtual void greet() const override { std::cout << "Rectangle\n"; };
  virtual double area() const override { return b * h; };
  virtual ~Rectangle() override = default;
  double b, h;
};

struct Triangle : public Shape{
  virtual void greet() const override { std::cout << "Triangle\n"; };
  virtual double area() const override { return b * h / 2.0; };
  virtual ~Triangle() override = default;
  double b, h;
};

struct Circle : public Shape {
  virtual void greet() const override { std::cout << "Circle\n"; };
  virtual double area() const override { return 3.1415 * radius * radius; };
  virtual ~Circle() override  = default;
  double radius;
};

using AnyShape = std::variant<Square, Rectangle, Triangle, Circle>;

AnyShape make_random_shape() {
  std::random_device r;
  std::default_random_engine engine(r());
  std::uniform_int_distribution<int> shape_id_dist(0, 3);
  std::uniform_real_distribution<double> uniform_dist(0, 10.);

  AnyShape shape;
  const auto id = shape_id_dist(engine);

  if(id == 0) {
    Square square;
    square.side = uniform_dist(engine);
    shape = square;
  } else if (id == 1) {
    Rectangle rectangle;
    rectangle.b = uniform_dist(engine);
    rectangle.h = uniform_dist(engine);
    shape = rectangle;
  } else if (id == 2) {
    Triangle triangle ;
    triangle.b = uniform_dist(engine);
    triangle.h = uniform_dist(engine);
    shape = triangle;
  } else if (id == 3) {
    Circle circle;
    circle.radius = uniform_dist(engine);
    shape = circle;
  } else {
    std::cout << "ERROR: Unknown shape id: " << id << std::endl;
    std::exit;
  }

  return shape;
}

///////////////////////////////////////////////////////////////////////////////

template <class Derived>
struct ShapeCRTP {
  double area() const { return static_cast<const Derived*>(this)->area_impl(); };
};

struct VoidShapeCRTP : public ShapeCRTP<VoidShapeCRTP> {
  double area_impl() const;
};

double VoidShapeCRTP::area_impl() const {
  return 0;
}

struct SquareCRTP : public ShapeCRTP<SquareCRTP> {
  double area_impl() const { return side * side; };
  double side;
};

struct RectangleCRTP : public ShapeCRTP<RectangleCRTP> {
  double area_impl() const { return b * h; };
  double b, h;
};

struct TriangleCRTP : public ShapeCRTP<TriangleCRTP>{
  double area_impl() const { return b * h / 2.0; };
  double b, h;
};

struct CircleCRTP : public ShapeCRTP<CircleCRTP> {
  double area_impl() const { return 3.1415 * radius * radius; };
  double radius;
};

using AnyShapeCRTP = std::variant<VoidShapeCRTP, SquareCRTP, RectangleCRTP, TriangleCRTP, CircleCRTP>;

AnyShapeCRTP make_random_shape_crtp() {
  std::random_device r;
  std::default_random_engine engine(r());
  std::uniform_int_distribution<int> shape_id_dist(0, 3);
  std::uniform_real_distribution<double> uniform_dist(0, 10.);

  AnyShapeCRTP shape;
  const auto id = shape_id_dist(engine);

  if(id == 0) {
    SquareCRTP square;
    square.side = uniform_dist(engine);
    shape = square;
  } else if (id == 1) {
    RectangleCRTP rectangle;
    rectangle.b = uniform_dist(engine);
    rectangle.h = uniform_dist(engine);
    shape = rectangle;
  } else if (id == 2) {
    TriangleCRTP triangle ;
    triangle.b = uniform_dist(engine);
    triangle.h = uniform_dist(engine);
    shape = triangle;
  } else if (id == 3) {
    CircleCRTP circle;
    circle.radius = uniform_dist(engine);
    shape = circle;
  } else {
    std::cout << "ERROR: Unknown shape id: " << id << std::endl;
    std::exit;
  }

  return shape;
}

///////////////////////////////////////////////////////////////////////////////


using namespace std::chrono;


int main(int argc, char **argv) {
  if(argc < 2)
    std::exit(-1);
  const size_t n_shapes = std::atoll(argv[1]);
  std::vector<Rectangle> rectangles;
  std::vector<RectangleCRTP> crtp_rectangles;
  std::vector<std::shared_ptr<Shape>> shapes;
  std::vector<AnyShape> variant_shapes;
  std::vector<AnyShapeCRTP> crtp_variant_shapes;
  // initialize
  std::random_device r;
  std::default_random_engine engine(r());
  std::uniform_real_distribution<double> uniform_dist(0, 10.);
  for(size_t i = 0; i < n_shapes; ++i) {
    Rectangle r;
    r.b = uniform_dist(engine);
    r.h = uniform_dist(engine);
    rectangles.push_back(r);

    RectangleCRTP rcrtp;
    rcrtp.h = r.h;
    rcrtp.b = r.b;
    crtp_rectangles.push_back(rcrtp);

    const AnyShape s = make_random_shape();
    std::visit([&](auto&& arg){ shapes.push_back(std::make_shared<std::remove_cvref_t<decltype(arg)>>(arg)); }, s);
    variant_shapes.push_back(s);
    crtp_variant_shapes.push_back(make_random_shape_crtp());
  }
  // compute
  for(int i = 0; i < 3; ++i) {
    {
      const auto t0 = high_resolution_clock::now();
      double tot_area = 0.0;
      for(const auto &shape : rectangles) {
        tot_area += shape.area();
      }
      const auto t1 = high_resolution_clock::now();
      const auto dt = duration_cast<microseconds>(t1 - t0).count();
      std::cout << "vector<Rectangle>                   "<< dt << " " << tot_area << std::endl;
    }
    {
      const auto t0 = high_resolution_clock::now();
      double tot_area = 0.0;
      for(const auto &shape : crtp_rectangles) {
        tot_area += shape.area();
      }
      const auto t1 = high_resolution_clock::now();
      const auto dt = duration_cast<microseconds>(t1 - t0).count();
      std::cout << "vector<RectangleCRTP>               "<< dt << " " << tot_area << std::endl;
    }
    {
      const auto t0 = high_resolution_clock::now();
      double tot_area = 0.0;
      for(const auto &shape : shapes) {
        tot_area += shape->area();
      }
      const auto t1 = high_resolution_clock::now();
      const auto dt = duration_cast<microseconds>(t1 - t0).count();
      std::cout << "vector<Shapes>                      " << dt << " " << tot_area << std::endl;
    }
        {
      const auto t0 = high_resolution_clock::now();
      double tot_area = 0.0;
      for(const auto &shape : variant_shapes) {
        std::visit([&](auto&& arg){ tot_area += arg.area(); }, shape);
      }
      const auto t1 = high_resolution_clock::now();
      const auto dt = duration_cast<microseconds>(t1 - t0).count();
      std::cout << "vector<variant<...Shapes>>          "<<dt << " " << tot_area << std::endl;
    }
    {
      const auto t0 = high_resolution_clock::now();
      double tot_area = 0.0;
      for(const auto &shape : crtp_variant_shapes) {
        std::visit([&](auto&& arg){ tot_area += arg.area_impl(); }, shape);
      }
      const auto t1 = high_resolution_clock::now();
      const auto dt = duration_cast<microseconds>(t1 - t0).count();
      std::cout <<  "vector<variant<...ShapesCRTP>>      "<<dt << " " << tot_area << std::endl;
    }
    {
      const auto t0 = high_resolution_clock::now();
      double tot_area = 0.0;
      for(const auto &shape : variant_shapes) {
        // this is undefined behaviour, should be careful
        // at this point should prefer a union
        const auto &shape_ref = (const Shape &) shape;
        tot_area += shape_ref.area();
      }
      const auto t1 = high_resolution_clock::now();
      const auto dt = duration_cast<microseconds>(t1 - t0).count();
      std::cout << "vector<variant<...Shapes>> [UB]     "<<dt << " " << tot_area << std::endl;
    }

    std::cout << "-------------------------" << std::endl;
  }




  return 0;
}