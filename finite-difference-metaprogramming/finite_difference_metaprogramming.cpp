#include <array>
#include <iostream>
#include <string_view>

template <std::string_view const&... Strs>
struct join
{
    // Join all strings into a single std::array of chars
    static constexpr auto impl() noexcept
    {
        constexpr std::size_t len = (Strs.size() + ... + 0);
        std::array<char, len + 1> arr{};
        auto append = [i = 0, &arr](auto const& s) mutable {
            for (auto c : s) arr[i++] = c;
        };
        (append(Strs), ...);
        arr[len] = 0;
        return arr;
    }
    // Give the joined string static storage
    static constexpr auto arr = impl();
    // View as a std::string_view
    static constexpr std::string_view value {arr.data(), arr.size() - 1};
};
// Helper to get the value out
template <std::string_view const&... Strs>
static constexpr auto join_v = join<Strs...>::value;

static constexpr std::string_view x = "x";
static constexpr std::string_view hp = " + h";
static constexpr std::string_view hm = " - h";
static constexpr std::string_view lb = "(";
static constexpr std::string_view rb = ")";
static constexpr std::string_view m = " - ";
static constexpr std::string_view f = "f";
static constexpr std::string_view h = " / h";

namespace Differentiation
{
  class BACKWARD; // Forward declaration.

  class FORWARD
  {
  public:
    using OTHER_TYPE = BACKWARD;
  };

  class BACKWARD
  {
  public:
    using OTHER_TYPE = FORWARD;
  };
} // namespace Differentiation

template <unsigned int N,
          const std::string_view &X,
          class DifferenceType = Differentiation::FORWARD>
struct NthDerivative {
static constexpr auto impl() {
  if constexpr (std::is_same_v<Differentiation::FORWARD, DifferenceType>) {
    return join_v<
      lb,
      NthDerivative<N - 1, join_v<X, hp>, typename DifferenceType::OTHER_TYPE>::value,
      m,
      NthDerivative<N - 1, X, typename DifferenceType::OTHER_TYPE>::value,
      rb,
      h
    >;
  } else {
    return join_v<
      lb,
      NthDerivative<N - 1, X, typename DifferenceType::OTHER_TYPE>::value,
      m,
      NthDerivative<N - 1, join_v<X, hm>, typename DifferenceType::OTHER_TYPE>::value,
      rb,
      h
    >;
  }
}
static constexpr std::string_view value = impl();
};

template <const std::string_view &X,
          class DifferenceType>
struct NthDerivative<0u, X, DifferenceType> {
static constexpr auto impl() {
  return join_v<f, lb, X, rb>;
}
static constexpr std::string_view value = impl();
};

int main() {
  std::cout << NthDerivative<0, x>::value << '\n';
  std::cout << NthDerivative<1, x>::value << '\n';
  std::cout << NthDerivative<2, x>::value << '\n';
}