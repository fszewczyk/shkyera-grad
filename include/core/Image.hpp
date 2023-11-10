/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

#include "Vector.hpp"

namespace shkyera {

class Image {
  private:
    std::vector<uint8_t> _data;

  public:
    Image() = default;
    Image(std::string filename, bool grayscale = true);

    template <typename T> Vector<T> flatten(size_t takeEvery = 1) const;
};

Image::Image(std::string filename, bool grayscale) {
    int width, height, channels;
    uint8_t *imageData = nullptr;

    if (grayscale)
        imageData = stbi_load(filename.c_str(), &width, &height, &channels, 1);
    else
        imageData = stbi_load(filename.c_str(), &width, &height, &channels, 3);

    if (!imageData) {
        std::cerr << "Error loading image: " << filename << std::endl;
        return;
    }

    if (grayscale)
        _data.assign(imageData, imageData + (width * height));
    else
        _data.assign(imageData, imageData + (width * height * 3));

    stbi_image_free(imageData);
}

template <typename T> Vector<T> Image::flatten(size_t takeEvery) const {
    std::vector<T> converted;
    converted.reserve(_data.size());
    for (size_t i = 0; i < _data.size(); i += takeEvery)
        converted.push_back(static_cast<T>(_data[i]));
    return Vector<T>::of(converted);
}

} // namespace shkyera
