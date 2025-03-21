#pragma once

#include "random.h"
#include <algorithm>
#include <array>

namespace Layer {

// I - number of inputs to layer
// N - number of neurons in layer
template <size_t I, size_t N> class Dense {
public:
  Dense();

  // perform forward pass on given inputs
  // save inputs and outputs in member variables
  void forward(const double inputs[]);

  const std::array<double, N> &getOutput() const { return m_output; }

private:
  const double *m_inputs{nullptr};
  std::array<std::array<double, I>, N> m_weights{};
  std::array<double, N> m_biases{};
  std::array<double, N> m_output{};
};

template <size_t I, size_t N> Dense<I, N>::Dense() {
  // Generate random values for each of the weights
  for (size_t neuron{}; neuron < N; ++neuron)
    std::generate_n(m_weights[neuron].begin(), I,
                    []() { return 0.01 * Random::getNormal(); });
  // Fill bias array with 0s
  std::fill_n(m_biases.begin(), N, 0);
}

template <size_t I, size_t N>
void Dense<I, N>::forward(const double inputs[I]) {
  delete m_inputs;
  m_inputs = inputs;

  for (size_t neuron{}; neuron < N; ++neuron) {
    double weightInputSum{};
    for (size_t input{}; input < I; ++input)
      weightInputSum += m_weights[neuron][input] * m_inputs[input];
    m_output[neuron] = weightInputSum + m_biases[neuron];
  }
}
} // namespace Layer
