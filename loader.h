#pragma once

#include "image.h"

#include <fstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

class MNistLoader {
public:
  // a type which contains a vector of labels and a matching vector of images
  using DataPair =
      std::tuple<std::vector<unsigned char>, std::vector<MNistImage>>;

  MNistLoader(std::string_view trainingLabelsPath,
              std::string_view trainingImagesPath,
              std::string_view testingLabelsPath,
              std::string_view testingImagesPath)
      : m_trainingLabelsPath{trainingLabelsPath},
        m_trainingImagesPath{trainingImagesPath},
        m_testingLabelsPath{testingLabelsPath},
        m_testingImagesPath{testingImagesPath} {}

  std::array<DataPair, 2> loadData() const {
    auto train{loadImages(m_trainingLabelsPath, m_trainingImagesPath)};
    auto test{loadImages(m_testingLabelsPath, m_testingImagesPath)};

    return std::array{train, test};
  }

private:
  std::string m_trainingLabelsPath{};
  std::string m_trainingImagesPath{};
  std::string m_testingLabelsPath{};
  std::string m_testingImagesPath{};

  static DataPair loadImages(std::string_view labelsPath,
                             std::string_view imagesPath);

  static std::vector<unsigned char> loadLabelsFile(std::string_view labelsPath);

  static std::vector<MNistImage> loadImagesFile(std::string_view imagesPath);

  static std::vector<unsigned char> readBytes(std::ifstream &file,
                                              const unsigned int length,
                                              const std::string &filename = "");

  static unsigned int readU32(std::ifstream &file,
                              const std::string &filename = "");
};
