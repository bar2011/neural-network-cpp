#include "layer.h"
#include "loader.h"
#include <algorithm>
#include <iostream>
#include <string_view>
#include <tuple>

std::array<MNistLoader::DataPair, 2> loadData() {
  std::string_view trainingLabelsPath{"./data/train-labels-idx1-ubyte"};
  std::string_view trainingImagesPath{"./data/train-images-idx3-ubyte"};
  std::string_view testingLabelsPath{"./data/t10k-labels-idx1-ubyte"};
  std::string_view testingImagesPath{"./data/t10k-images-idx3-ubyte"};
  MNistLoader *loader{new MNistLoader(trainingLabelsPath, trainingImagesPath,
                                      testingLabelsPath, testingImagesPath)};

  auto data{loader->loadData()};

  return data;
}

int main() {
  auto mnistData{loadData()};

  // rows and columns in each image
  constexpr int rows{28};
  constexpr int cols{28};

  auto testLayer{new Layer::Dense<rows * cols, 10>()};
  testLayer->forward(std::get<1>(mnistData[0])[0].data().data());

  std::get<1>(mnistData[0])[0].print();
  std::cout << '\n';

  std::cout << "guess: " << std::max_element(testLayer->getOutput().begin(), testLayer->getOutput().end()) - testLayer->getOutput().begin() << '\n';
  std::cout << "real: " << static_cast<int>(std::get<0>(mnistData[0])[0]) << '\n';

  return 0;
}
