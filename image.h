#pragma once

#include <vector>

class MNistImage {
private:
  using ImageVector = std::vector<std::vector<unsigned char>>;

public:
  MNistImage(const unsigned char *dataBegin, const unsigned char *dataEnd,
             const unsigned int rows, const unsigned int cols);

  void print() const;

private:
  ImageVector m_image{};

  void static printGrayscaleColor(unsigned int shade);
};
