#pragma once

#include <string>

#include <glm/ext/vector_float3.hpp>

#include "materials/texture.hpp"

namespace raytracer::materials {

class ImageTexture : public Texture {
  private:
    int width = 0;
    int height = 0;
    int num_channels = 0;
    float* fdata;

  public:
    ImageTexture(const ImageTexture&) = delete;
    ImageTexture(ImageTexture&&) = delete;
    ImageTexture& operator=(const ImageTexture&) = delete;
    ImageTexture& operator=(ImageTexture&&) = delete;
    explicit ImageTexture(const std::string& filename);
    ~ImageTexture() override;
    glm::vec3 value(float u, float v, const glm::vec3& p) const override;
};

} // namespace raytracer::materials
