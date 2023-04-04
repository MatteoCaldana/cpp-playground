// build asmjit and mathpresso as usual with cmake in the same parent directory
// g++ main.cpp -I/mnt/d/Syncd/repos/mathpresso/src -Wl,-rpath=/mnt/d/Syncd/repos/mathpresso/build -L/mnt/d/Syncd/repos/mathpresso/build -lmathpresso
#include <mathpresso/mathpresso.h>
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
  mathpresso::Context ctx;
  mathpresso::Expression exp;

  ctx.addBuiltIns();
  ctx.addVariable("x", 0 * sizeof(double));
  mathpresso::Error err = exp.compile(ctx, "x*x*x + 5 * x + 3", mathpresso::kNoOptions);

  // Handle possible syntax or compilation error.
  if (err != mathpresso::kErrorOk) {
    printf("Expression Error: %u\n", err);
    return 1;
  }

  double x;
 
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  for(size_t i = 0; i < 1'000'000'000; ++i) {
    x = 1000000000.0 / double(i);
    const auto y = exp.evaluate(&x);
  }
  const auto t1 = high_resolution_clock::now();
  const auto dt = duration_cast<microseconds>(t1 - t0).count();
  std::cout << dt << " [µs]" << std::endl;

  return 0;
}
