#include "loader.h"
#include <string_view>

int main() {
  std::string_view trainingLabelsPath{"./data/train-labels-idx1-ubyte"};
  std::string_view trainingImagesPath{"./data/train-images-idx3-ubyte"};
  std::string_view testingLabelsPath{"./data/t10k-labels-idx1-ubyte"};
  std::string_view testingImagesPath{"./data/t10k-images-idx3-ubyte"};
  MNistLoader *loader{new MNistLoader(trainingLabelsPath, trainingImagesPath,
                                      testingLabelsPath, testingImagesPath)};

  loader->loadData();

  return 0;
}
