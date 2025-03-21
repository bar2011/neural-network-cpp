#pragma once

#include <vector>

class MNistImage {
public:
  MNistImage(const unsigned char *dataBegin, const unsigned char *dataEnd,
             const unsigned int rows, const unsigned int cols);

  void print() const;

  const std::vector<double> &data() const { return m_imageData; }

private:
  // saving as double and not unsigned char, because everything else uses double
  std::vector<double> m_imageData{};
  unsigned int m_rows{};
  unsigned int m_cols{};

  void static printGrayscaleColor(unsigned int shade);
};
