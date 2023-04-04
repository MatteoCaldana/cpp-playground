#include <chrono>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

// https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c

///////////////////////////////////////////////////////////////////////////////

class DynamicInterface {
 protected:
  uint64_t mCounter = 0;

 public:
  virtual void tick(uint64_t n) = 0;
  uint64_t getvalue() const { return mCounter; }
};

class DynamicImplementation : public DynamicInterface {
  virtual void tick(uint64_t n) { mCounter += n; }
};

template <int Scale>
class DynamicImplementationN : public DynamicInterface {
  virtual void tick(uint64_t n) { mCounter += Scale * n; }
};

void run_dynamic(DynamicInterface* obj, const unsigned N) {
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < i; ++j) {
      obj->tick(j);
    }
  }
}

void run_dynamic_v(const vector<shared_ptr<DynamicInterface>>& v,
                   const unsigned N) {
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < i; ++j) {
      for (const auto& obj : v) obj->tick(j);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Implementation, int Scale>
class CRTPInterface {
 protected:
  uint64_t mCounter = 0;

 public:
  void tick(uint64_t n) { impl().tick(n); }
  uint64_t getvalue() const { return mCounter; }

 private:
  Implementation& impl() { return *static_cast<Implementation*>(this); }
};

class CRTPImplementation : public CRTPInterface<CRTPImplementation, 1> {
 public:
  void tick(uint64_t n) { mCounter += n; }
};

template <int Scale>
class CRTPImplementationN : public CRTPInterface<CRTPImplementation, Scale> {
 public:
  void tick(uint64_t n) { mCounter += Scale * n; }
};

template <typename Implementation>
void run_crtp(CRTPInterface<Implementation, 1>* obj, const unsigned N) {
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < i; ++j) {
      obj->tick(j);
    }
  }
}

typedef tuple<
    CRTPImplementationN<1>, CRTPImplementationN<2>, CRTPImplementationN<3>,
    CRTPImplementationN<4>, CRTPImplementationN<5>, CRTPImplementationN<6>,
    CRTPImplementationN<7>, CRTPImplementationN<8>, CRTPImplementationN<9>>
    CRTP_Vector;

// https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
template <std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type for_each(
    std::tuple<Tp...>&, FuncT)  // Unused arguments are given no names.
{}

template <std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if <
    I<sizeof...(Tp), void>::type for_each(std::tuple<Tp...>& t, FuncT f) {
  f(std::get<I>(t));
  for_each<I + 1, FuncT, Tp...>(t, f);
}

void run_crtp_v(CRTP_Vector& v, const unsigned N) {
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < i; ++j) {
      for_each(v, [j](auto& arg) { arg.tick(j); });  // need c++11
      // std::apply([j](auto&&... args) { ((args.tick(j)), ...); }, v);//c++17
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

void benckmark1() {
  constexpr unsigned N = 40000;

  DynamicImplementation di;
  auto t1 = high_resolution_clock::now();
  run_dynamic(&di, N);
  auto t2 = high_resolution_clock::now();
  cout << di.getvalue()
       << "Dyna: " << duration_cast<milliseconds>(t2 - t1).count() << std::endl;

  CRTPImplementation ci;
  auto t3 = high_resolution_clock::now();
  run_crtp(&ci, N);
  auto t4 = high_resolution_clock::now();
  cout << ci.getvalue()
       << "CRTP: " << duration_cast<milliseconds>(t4 - t3).count() << std::endl;
}

void benckmark2() {
  constexpr unsigned N = 15000;

  vector<shared_ptr<DynamicInterface>> dynamicv;
  dynamicv.emplace_back(make_shared<DynamicImplementationN<1>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<2>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<3>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<4>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<5>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<6>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<7>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<8>>());
  dynamicv.emplace_back(make_shared<DynamicImplementationN<9>>());
  auto t1 = high_resolution_clock::now();
  run_dynamic_v(dynamicv, N);
  auto t2 = high_resolution_clock::now();
  cout << "Dyna: " << duration_cast<milliseconds>(t2 - t1).count() << std::endl;

  auto crtpv =
      std::make_tuple(CRTPImplementationN<1>(), CRTPImplementationN<2>(),
                      CRTPImplementationN<3>(), CRTPImplementationN<4>(),
                      CRTPImplementationN<5>(), CRTPImplementationN<6>(),
                      CRTPImplementationN<7>(), CRTPImplementationN<8>(),
                      CRTPImplementationN<9>());
  auto t3 = high_resolution_clock::now();
  run_crtp_v(crtpv, N);
  auto t4 = high_resolution_clock::now();
  cout << "CRTP: " << duration_cast<milliseconds>(t4 - t3).count() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

int main() {
  benckmark1();
  benckmark2();
  return 0;
}
