#include <chrono>

#include "net.h"
#include "netcrtp.h"

using namespace std;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

// TODO:
// - crtp
// - do not use size but start-end

int main() {
  Eigen::setNbThreads(4);

  auto t1 = high_resolution_clock::now();
  netcrtp::test(10000);
  auto t2 = high_resolution_clock::now();
  auto ms_int = duration_cast<milliseconds>(t2 - t1);
  std::cout << ms_int.count() << "ms\n";

  return 0;
}
