#ifndef NETCRTP_H
#define NETCRTP_H

#include <Eigen/Core>
#include <cmath>
#include <iostream>
#include <vector>

namespace netcrtp {

template <typename Implementation>
class Layer {
 protected:
  Eigen::VectorXd m_out;
  const Eigen::VectorXd *m_in;

 public:
  void forward(const Eigen::VectorXd &in) { impl().forward(in); };
  void backward(double *buff, size_t n) const { impl().backward(buff, n); };
  long long n_params() const { return impl().n_params(); };
  const Eigen::VectorXd &out() const { return m_out; }
  const Eigen::VectorXd *in() const { return m_in; }
  void setInput(const Eigen::VectorXd *in) { m_in = in; };

 private:
  Implementation &impl() { return *static_cast<Implementation *>(this); }
};

class Linear : public Layer<Linear> {
 public:
  Linear(long long n, long long m)
      : m_weights(0.1 * Eigen::MatrixXd::Ones(m, n)),
        m_bias(0.1 * Eigen::VectorXd::Ones(m)){};

  void forward(const Eigen::VectorXd &in) { m_out = m_weights * in + m_bias; }

  void backward(double *buff, size_t n) const {
    for (auto i = 0ll; i < m_weights.size(); ++i)
      buff[i] *= (*m_in)[i % (*m_in).size()];
    for (auto i = n_params(); i < n; ++i)
      buff[i] *= m_weights.data()[(i - n_params()) % m_weights.size()];
  }

  long long n_params() const { return m_weights.size() + m_bias.size(); };

 private:
  Eigen::MatrixXd m_weights;
  Eigen::VectorXd m_bias;
};

class Tanh : public Layer<Tanh> {
 public:
  void forward(const Eigen::VectorXd &in) {
    m_out = in.unaryExpr([](auto x) { return std::tanh(x); });
  }

  void backward(double *buff, size_t n) const {
    for (auto i = 0ull; i < n; ++i) {
      const auto tmp = std::tanh((*m_in)[i % (*m_in).size()]);
      buff[i] *= 1 - tmp * tmp;
    }
  }

  long long n_params() const { return 0; }
};

template <std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type for_each(
    std::tuple<Tp...> &, FuncT)  // Unused arguments are given no names.
{}

template <std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if <
    I<sizeof...(Tp), void>::type for_each(std::tuple<Tp...> &t, FuncT f) {
  f(std::get<I>(t));
  for_each<I + 1, FuncT, Tp...>(t, f);
}

template <class... LayerTypes>
class Net {
 public:
  Net(LayerTypes... layers) : m_net(std::make_tuple(layers...)) {
    m_nparams = 0;
    std::apply([this](auto &&... t) { ((m_nparams += t.n_params()), ...); },
               m_net);
    const Eigen::VectorXd *tmp = nullptr;
    for_each(m_net, [&tmp](auto &l) {
      l.setInput(tmp);
      tmp = &l.out();
    });
  };

  const Eigen::VectorXd &forward(const Eigen::VectorXd &in) {
    std::get<0>(m_net).setInput(&in);
    std::apply([](auto &&... t) { ((t.forward(*t.in())), ...); }, m_net);
    return std::get<sizeof...(LayerTypes) - 1>(m_net).out();
  }

  Eigen::VectorXd backward() {
    long long n = 0;
    Eigen::VectorXd result(Eigen::VectorXd::Ones(m_nparams));
    for_each(m_net, [&n, &result](auto &l) {
      l.backward(result.data() + n, result.size() - n);
      n += l.n_params();
    });
    return result;
  }

 private:
  std::tuple<LayerTypes...> m_net;
  long long m_nparams;
  const Eigen::VectorXd *m_in;
};

void test(int N) {
  auto net = Net(Linear(4, 32), Tanh(), Linear(32, 32), Tanh(), Linear(32, 1));
  for (auto i = 0; i < N; ++i) {
    const auto x = Eigen::VectorXd::Random(4);
    const auto y = net.forward(x);
    const auto grad = net.backward();
  }
}

}  // namespace netcrtp

#endif  // NETCRTP_H
