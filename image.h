#pragma once

#include <iostream>
#include <vector>

class MNistImage {
private:
  using ImageVector = std::vector<std::vector<unsigned char>>;

public:
  MNistImage(const unsigned char *dataBegin, const unsigned char *dataEnd,
             const unsigned int rows, const unsigned int cols) {
    if (dataEnd - dataBegin != rows * cols)
      throw "Error on MNistImage(data, rows, cols): data size provided doesn't "
            "match rows and cols";

    m_image.reserve(rows * cols);

    for (unsigned int row{}; row < rows; ++row) {
      m_image.push_back(std::vector<unsigned char>(cols));
      for (unsigned int col{}; col < cols; ++col)
        m_image[row][col] = dataBegin[row * rows + col];
    }
  }

  void print() const {
    for (size_t row{}; row < m_image.size(); ++row) {
      for (size_t col{}; col < m_image[row].size(); ++col)
        printGrayscaleColor(static_cast<unsigned int>(m_image[row][col]));
      std::cout << '\n';
    }
  }

private:
  ImageVector m_image{};

  void static printGrayscaleColor(unsigned int shade) {
    if (shade > 255)
      shade = 255;

    std::cout << "\033[38;2;" << shade << ';' << shade << ';' << shade
              << "m█\033[0m";
  }
};
