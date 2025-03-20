#include "loader.h"
#include "image.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

MNistLoader::DataPair MNistLoader::loadImages(std::string_view labelsPath,
                                              std::string_view imagesPath) {
  std::vector<unsigned char> labels{loadLabelsFile(labelsPath)};
  std::vector<MNistImage> images{loadImagesFile(imagesPath)};

  return std::make_tuple(std::move(labels), std::move(images));
}

std::vector<unsigned char>
MNistLoader::loadLabelsFile(std::string_view labelsPath) {
  std::ifstream labelsFile{labelsPath, std::ios::binary | std::ios::in};

  // testing number which should always be 2049
  unsigned int magicNumber{
      readU32(labelsFile, static_cast<std::string>(labelsPath))};
  if (magicNumber != 2049)
    throw "Invalid file format for image label file: " +
        static_cast<std::string>(labelsPath);

  // Number of image labels (each one is a byte, so also length of remaining
  // file)
  unsigned int size{readU32(labelsFile, static_cast<std::string>(labelsPath))};

  // Actual data
  std::vector<unsigned char> labels{
      readBytes(labelsFile, size, static_cast<std::string>(labelsPath))};

  return labels;
}

std::vector<MNistImage>
MNistLoader::loadImagesFile(std::string_view imagesPath) {
  std::ifstream imagesFile{imagesPath, std::ios::binary | std::ios::in};

  // testing number which should always be 2051
  unsigned int magicNumber{
      readU32(imagesFile, static_cast<std::string>(imagesPath))};
  if (magicNumber != 2051)
    throw "Invalid file format for image file: " +
        static_cast<std::string>(imagesPath);

  // Number of images
  unsigned int size{readU32(imagesFile, static_cast<std::string>(imagesPath))};

  // Number of rows in each image
  unsigned int rows{readU32(imagesFile, static_cast<std::string>(imagesPath))};

  // Number of columns in each image
  unsigned int cols{readU32(imagesFile, static_cast<std::string>(imagesPath))};

  // Plain image data
  std::vector<unsigned char> imageData{readBytes(
      imagesFile, size * rows * cols, static_cast<std::string>(imagesPath))};

  std::vector<MNistImage> images{};
  images.reserve(size);

  for (unsigned int i{}; i < size; ++i) {
    images.push_back(MNistImage(&imageData[i * rows * cols],
                                &imageData[(i + 1) * rows * cols], rows, cols));
  }

  return images;
}

std::vector<unsigned char> MNistLoader::readBytes(std::ifstream &file,
                                                  const unsigned int length,
                                                  const std::string &filename) {
  std::vector<char> bytes(length);
  if (!file.read(&bytes[0], length))
    throw "Error on readBytes(): Can't read file " + filename +
        ": file size smaller then expected.";

  std::vector<unsigned char> unsignedBytes(length);

  std::transform(bytes.begin(), bytes.end(), unsignedBytes.begin(),
                 [](char c) { return static_cast<unsigned char>(c); });

  return unsignedBytes;
}

unsigned int MNistLoader::readU32(std::ifstream &file,
                                  const std::string &filename) {
  char bytes[4]{};
  if (!file.read(bytes, 4))
    throw "Error on readU32() : Can't read file " + filename + ": reached end";
  unsigned int value{
      static_cast<unsigned int>(static_cast<unsigned char>(bytes[3]) |
                                (static_cast<unsigned char>(bytes[2]) << 8) |
                                (static_cast<unsigned char>(bytes[1]) << 16) |
                                (static_cast<unsigned char>(bytes[0]) << 24))};
  return value;
}
