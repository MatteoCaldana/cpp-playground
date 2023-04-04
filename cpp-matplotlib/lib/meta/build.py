import matplotlib.pyplot as plt

LIB = """
#include <pybind11/embed.h> 
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace py::literals;

class pyplot {{
public:
  pyplot() : guard() {{}};

  {}

private:
  py::scoped_interpreter guard;
  py::object plt = py::module_::import("matplotlib.pyplot");
}};
"""

METHOD = """
template<typename ...Args>
py::object {}(Args&&... args) {{ return plt.attr("{}")(std::forward<Args>(args)...); }};

"""

if __name__ == "__main__":
  funcs = [f for f in dir(plt) if callable(plt.__getattribute__(f))]

  methods = "".join([METHOD.format(f, f) for f in funcs])
  lib = LIB.format(methods)

  with open("../src/cppmatplot.hpp", "w") as fp:
    fp.write(lib)