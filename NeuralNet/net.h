#ifndef NET_H
#define NET_H

#include <Eigen/Core>
#include <cmath>
#include <iostream>
#include <vector>

namespace net {

class Layer {
 public:
  virtual ~Layer() = default;

  virtual void forward(const Eigen::VectorXd &in) = 0;
  virtual void backward(double *buff, size_t n,
                        const Eigen::VectorXd &in) const = 0;
  virtual long long n_params() const = 0;

  Eigen::VectorXd out;
};

class Linear : public Layer {
 public:
  Linear(long long n, long long m)
      : m_weights(0.1 * Eigen::MatrixXd::Ones(m, n)),
        m_bias(0.1 * Eigen::VectorXd::Ones(m)){};

  virtual void forward(const Eigen::VectorXd &in) override {
    out = m_weights * in + m_bias;
  }

  virtual void backward(double *buff, size_t n,
                        const Eigen::VectorXd &in) const override {
    for (auto i = 0ll; i < m_weights.size(); ++i) buff[i] *= in[i % in.size()];
    for (auto i = n_params(); i < n; ++i)
      buff[i] *= m_weights.data()[(i - n_params()) % m_weights.size()];
  }

  virtual long long n_params() const override {
    return m_weights.size() + m_bias.size();
  };

 private:
  Eigen::MatrixXd m_weights;
  Eigen::VectorXd m_bias;
};

class Tanh : public Layer {
  virtual void forward(const Eigen::VectorXd &in) override {
    out = in.unaryExpr([](auto x) { return std::tanh(x); });
  }

  virtual void backward(double *buff, size_t n,
                        const Eigen::VectorXd &in) const override {
    for (auto i = 0ull; i < n; ++i) {
      const auto tmp = std::tanh(in[i % in.size()]);
      buff[i] *= 1 - tmp * tmp;
    }
  }

  virtual long long n_params() const override { return 0; }
};

class Net : public std::vector<std::unique_ptr<Layer>> {
 public:
  Net() = default;

  const Eigen::VectorXd &forward(const Eigen::VectorXd &in) {
    m_in = &in;
    operator[](0)->forward(in);
    for (size_t i = 1; i < size(); ++i) {
      operator[](i)->forward(operator[](i - 1)->out);
    }
    return operator[](size() - 1)->out;
  }

  Eigen::VectorXd backward() const {
    long long n = 0;
    Eigen::VectorXd result(Eigen::VectorXd::Ones(n_params()));
    for (size_t i = size() - 1; i > 0; --i) {
      operator[](i)->backward(result.data() + n,
                              result.size() - n, operator[](i - 1)->out);
      n += operator[](i)->n_params();
    }
    operator[](0)->backward(result.data() + n, result.size() - n, *m_in);
    return result;
  }

  long long n_params() const {
    long long n_params = 0;
    for (size_t i = 0; i < size(); ++i) {
      n_params += operator[](i)->n_params();
    }
    return n_params;
  }

 protected:
  const Eigen::VectorXd *m_in;
};

void test(int N) {
  Net n;
  n.push_back(std::make_unique<Linear>(4, 32));
  n.push_back(std::make_unique<Tanh>());
  n.push_back(std::make_unique<Linear>(32, 1));
  n.push_back(std::make_unique<Tanh>());

  for (auto i = 0; i < N; ++i) {
    const auto x = Eigen::VectorXd::Random(4);
    const auto y = n.forward(x);
    const auto grad = n.backward();
  }
}
}  // namespace net

#endif // NET_H
