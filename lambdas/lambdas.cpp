// https://www.youtube.com/watch?v=xBAduq0RGes
#include <functional>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class NoisyString {
public:
  NoisyString(const std::string &s) : m_s(s) {std::cout << "c1\n";}
  NoisyString(const char *s) : m_s(s) {std::cout << "c2\n";}
  NoisyString& operator=(const std::string& s) { m_s = s; std::cout << "ca\n"; return *this;}
  NoisyString(std::string &&s) : m_s(std::move(s)) {std::cout << "m\n";}
  NoisyString& operator=(std::string&& s) { m_s = std::move(s); std::cout << "ma\n";return *this; }

private:
  std::string m_s; 
};

template<typename T>
constexpr auto c_cast = [](auto x) {return (T) x; };

template<typename... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};


int main() {
  // do not need to capture not odr
  constexpr int i = 42;
  [] {std::cout << i << '\n';}();

  // call once
  struct X {
    X() {
      static auto _ = [] {std::cout << "Call once for all\n"; return 0; }();
    }
  };
  X x1, x2;

  // perfect forwarding
  std::vector<NoisyString> v;
  auto f = [&v](auto &&item) {v.push_back(std::forward<decltype(item)>(item));};
  const std::string s1 = "s1";
  std::string s2 = "s2";
  f(s1);
  f(std::move(s1));
  f(s2);
  f(std::move(s2));
  f("s3");

  // variadic lambda
  auto fv = [](auto &&... args) {
    (std::cout << ... << args) << '\n'; // fold expression
  };
  fv(42, "Hello", 1.5);

  // functional madness
  auto twice = [](auto &&f) { return [=]{f(); f(); std::cout << '\n';};};
  auto print_hihi = twice([]{std::cout << "hi";});
  print_hihi();

  // template lambda
  const auto u = c_cast<unsigned>(-1);
  std::cout << u << '\n';

  // init capture
  auto ptr = std::make_unique<NoisyString>("str");
  auto fi = [ptr=std::move(ptr)]{std::cout << ptr.get() << '\n';};
  std::cout << (ptr == nullptr) << '\n';
  fi();

  // advanced init capture
  const std::vector<std::string> vs = {"apple", "foobar", "orange", "lemon"};
  const std::string prefix = "foo";
  auto result = std::find_if(vs.begin(), vs.end(), 
    [str=prefix+"bar"] (const auto &s){ return s == str;} // save the temporary locally once for all
  );
  std::cout << (result != vs.cend()) << '\n';

  // constexpr
  auto fc = []() constexpr { return sizeof(void *); };
  std::array<int, fc()> a = {};

  // ctad
  overload f_ctad = {
    [](int i) { std::cout << i << " int\n"; },
    [](float f) { std::cout << f << " float\n"; },
  };
  f_ctad(42);
  f_ctad(1.2f);


  return 0;
}