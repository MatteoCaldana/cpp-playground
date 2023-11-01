#include <algorithm>
#include <iostream>

#include "GetPot"
#include "benchmarks.hpp"
#include "tests.hpp"

void print_help() {
  std::cout << "mmm: matrix-matrix multiplication implementations" << std::endl;
  std::cout << "  --mode, -m: specify if you want to run:" << std::endl;
  std::cout << "              - the 'test's " << std::endl;
  std::cout << "              - the 'benchmarks's" << std::endl;
  std::cout << "  --max_size, -n: log2(size) largest matrix" << std::endl;
  std::cout << "  --repetitions, -r: # repetitions for benchmark" << std::endl;
  std::cout << "  --precision, -p: floating precision (single/double/both)"
            << std::endl;
}

void argument_error(const std::string &arg = "") {
  std::cerr << "Error: " << arg << ". "
            << "Please use '--help' for details on how to use." << std::endl;
  std::exit(-1);
}

int main(int argc, char **argv) {
  GetPot cl(argc, argv);

  if (cl.search(2, "--help", "-h")) {
    print_help();
    return 0;
  }

  if (cl.search(2, "--mode", "-m")) {
    const std::string mode = cl.follow("", 2, "--mode", "-m");
    const std::string precision = cl.follow("both", 2, "--precision", "-p");
    if (mode == "test") {
      if (!(precision == "double" || precision == "single" ||
            precision == "both"))
        argument_error("unknows precision " + precision);
      if (precision == "double" || precision == "both") {
        std::cout << "Running in double precision" << std::endl;
        run_tests<double>();
      }
      if (precision == "single" || precision == "both") {
        std::cout << "Running in single precision" << std::endl;
        run_tests<float>();
      }
      return 0;
    }
    if (mode == "benchmark") {
      const int max_size = cl.follow(10, 2, "--max_size", "-n");
      const int reps = cl.follow(10, 2, "--repetitions", "-r");
      if (!(precision == "double" || precision == "single" ||
            precision == "both"))
        argument_error("unknows precision " + precision);
      if (precision == "double" || precision == "both") {
        // std::cout << "Running in double precision" << std::endl;
        run_benchamrks<double>(max_size, reps);
      }
      if (precision == "single" || precision == "both") {
        // std::cout << "Running in single precision" << std::endl;
        run_benchamrks<float>(max_size, reps);
      }
      return 0;
    }
    argument_error("invalid mode: " + mode);
  }
  argument_error("mode not found");
}