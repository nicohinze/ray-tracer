#include <string>

#include <glm/fwd.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "materials/image_texture.hpp"

namespace raytracer::materials {

ImageTexture::ImageTexture(const std::string& filename)
    : fdata(stbi_loadf(filename.c_str(), &width, &height, &num_channels, 3)) { // NOLINT(clang-analyzer-core.UndefinedBinaryOperatorResult)
}

ImageTexture::~ImageTexture() {
    STBI_FREE(fdata); // NOLINT(cppcoreguidelines-no-malloc,hicpp-no-malloc,cppcoreguidelines-owning-memory)
}

glm::vec3 ImageTexture::value(float u, float v, const glm::vec3& /*p*/) const {
    if (width <= 0 || height <= 0) {
        return {1, 0, 1};
    }
    const auto i = static_cast<int>(u * static_cast<float>(width - 1));
    const auto j = static_cast<int>((1 - v) * static_cast<float>(height - 1));
    const auto base = num_channels * (j * width + i);
    return {fdata[base], fdata[base + 1], fdata[base + 2]}; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}

} // namespace raytracer::materials
