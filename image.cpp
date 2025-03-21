#include "image.h"

#include <algorithm>
#include <iostream>

MNistImage::MNistImage(const unsigned char *dataBegin,
                       const unsigned char *dataEnd, const unsigned int rows,
                       const unsigned int cols)
    : m_rows{rows}, m_cols{cols} {
  if (dataEnd - dataBegin != rows * cols)
    throw "Error on MNistImage(data, rows, cols): data size provided doesn't "
          "match rows and cols";

  m_imageData.resize(rows * cols);
  std::copy(dataBegin, dataEnd, m_imageData.begin());
}

void MNistImage::print() const {
  for (size_t row{}; row < m_rows; ++row) {
    for (size_t col{}; col < m_cols; ++col)
      printGrayscaleColor(
          static_cast<unsigned int>(m_imageData[row * m_rows + col]));
    std::cout << '\n';
  }
}

void MNistImage::printGrayscaleColor(unsigned int shade) {
  if (shade > 255)
    shade = 255;

  std::cout << "\033[38;2;" << shade << ';' << shade << ';' << shade
            << "m█\033[0m";
}
