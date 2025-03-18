#pragma once

#include "image.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class MNistLoader {
public:
  MNistLoader(std::string_view trainingLabelsPath,
              std::string_view trainingImagesPath,
              std::string_view testingLabelsPath,
              std::string_view testingImagesPath)
      : m_trainingLabelsPath{trainingLabelsPath},
        m_trainingImagesPath{trainingImagesPath},
        m_testingLabelsPath{testingLabelsPath},
        m_testingImagesPath{testingImagesPath} {}
  void loadData() const {
    loadImages(m_trainingLabelsPath, m_trainingImagesPath);
    loadImages(m_testingLabelsPath, m_testingImagesPath);
  }

private:
  std::string m_trainingLabelsPath{};
  std::string m_trainingImagesPath{};
  std::string m_testingLabelsPath{};
  std::string m_testingImagesPath{};

  static void loadImages(std::string_view labelsPath,
                         std::string_view imagesPath) {
    std::vector<unsigned char> labels{loadLabelsFile(labelsPath)};
    std::vector<MNistImage> images{loadImagesFile(imagesPath)};
  }

  static std::vector<unsigned char>
  loadLabelsFile(std::string_view labelsPath) {
    std::ifstream labelsFile{labelsPath, std::ios::binary | std::ios::in};

    // testing number which should always be 2049
    unsigned int magicNumber{
        readU32(labelsFile, static_cast<std::string>(labelsPath))};
    if (magicNumber != 2049)
      throw "Invalid file format for image label file: " +
          static_cast<std::string>(labelsPath);

    // Number of image labels (each one is a byte, so also length of remaining
    // file)
    unsigned int size{
        readU32(labelsFile, static_cast<std::string>(labelsPath))};

    // Actual data
    std::vector<unsigned char> labels{
        readBytes(labelsFile, size, static_cast<std::string>(labelsPath))};

    return labels;
  }

  static std::vector<MNistImage> loadImagesFile(std::string_view imagesPath) {
    std::ifstream imagesFile{imagesPath, std::ios::binary | std::ios::in};

    // testing number which should always be 2051
    unsigned int magicNumber{
        readU32(imagesFile, static_cast<std::string>(imagesPath))};
    if (magicNumber != 2051)
      throw "Invalid file format for image file: " +
          static_cast<std::string>(imagesPath);

    // Number of images
    unsigned int size{
        readU32(imagesFile, static_cast<std::string>(imagesPath))};

    // Number of rows in each image
    unsigned int rows{
        readU32(imagesFile, static_cast<std::string>(imagesPath))};

    // Number of columns in each image
    unsigned int cols{
        readU32(imagesFile, static_cast<std::string>(imagesPath))};

    // Plain image data
    std::vector<unsigned char> imageData{readBytes(
        imagesFile, size * rows * cols, static_cast<std::string>(imagesPath))};

    std::vector<MNistImage> images{};
    images.reserve(size);

    for (unsigned int i{}; i < size; ++i) {
      images.push_back(MNistImage(&imageData[i * rows * cols],
                                  &imageData[(i + 1) * rows * cols], rows,
                                  cols));
      images[i].print();
      std::cout << '\n';
    }

    return images;
  }

  static std::vector<unsigned char>
  readBytes(std::ifstream &file, const unsigned int length,
            const std::string &filename = "") {
    std::vector<char> bytes(length);
    if (!file.read(&bytes[0], length))
      throw "Error on readBytes(): Can't read file " + filename +
          ": file size smaller then expected.";

    std::vector<unsigned char> unsignedBytes(length);

    std::transform(bytes.begin(), bytes.end(), unsignedBytes.begin(),
                   [](char c) { return static_cast<unsigned char>(c); });

    return unsignedBytes;
  }

  static unsigned int readU32(std::ifstream &file,
                              const std::string &filename = "") {
    char bytes[4]{};
    if (!file.read(bytes, 4))
      throw "Error on readU32() : Can't read file " + filename +
          ": reached end";
    unsigned int value{static_cast<unsigned int>(
        static_cast<unsigned char>(bytes[3]) |
        (static_cast<unsigned char>(bytes[2]) << 8) |
        (static_cast<unsigned char>(bytes[1]) << 16) |
        (static_cast<unsigned char>(bytes[0]) << 24))};
    return value;
  }
};
