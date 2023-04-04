#include <iostream>
#include <cmath>
#include <vector>
#include <utility>

#include <pybind11/embed.h> 
#include <pybind11/stl.h>

namespace py = pybind11;

struct pyplot {
  pyplot() : guard() {};
  py::scoped_interpreter guard;
  py::object plt = py::module_::import("matplotlib.pyplot");

  template<typename ...Params>
  py::object plot(Params&&... params) { return plt.attr("plot")(std::forward<Params>(params)...); };

  template<typename T>
  py::object savefig(T t) { return plt.attr("savefig")(t); };
};

int main() {
  std::vector<double> signal(1024);
  for (size_t i = 0; i < signal.size(); ++i)
    signal[i] = std::exp(i / -256.0) * std::cos(2 * M_PI * 8 * i / 1024.0);

  // Start the Python interpreter
  // py::scoped_interpreter guard{};
  using namespace py::literals;

  // Save the necessary local variables in a Python dict
  // py::dict locals = py::dict{"signal"_a = signal,};

  // Execute Python code, using the variables saved in `locals`
  // py::exec("import matplotlib.pyplot as plt");
  // py::exec("plt.plot(signal)", py::globals(), locals);
  // py::exec("plt.savefig('test.png')");

  // Execute from modules
  // auto plt = py::module_::import("matplotlib.pyplot");
  // plt.attr("plot")(signal, "color"_a="k");
  // plt.attr("savefig")("test2.png");
  pyplot plt;
  plt.plot(signal, "color"_a="r");
  plt.savefig("test3.png");

  return 0;
}