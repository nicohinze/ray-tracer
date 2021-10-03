#include "checker_texture.hpp"
#include "solid_color.hpp"

CheckerTexture::CheckerTexture(const glm::vec3& o, const glm::vec3& e)
    : odd(std::make_shared<SolidColor>(o)), even(std::make_shared<SolidColor>(e)) {}

CheckerTexture::CheckerTexture(std::shared_ptr<Texture> o, std::shared_ptr<Texture> e)
    : odd(std::move(o)), even(std::move(e)) {}

glm::vec3 CheckerTexture::value(float u, float v, const glm::vec3& p) const {
    constexpr auto repeat_factor = 10.0F;
    const auto sin = std::sin(repeat_factor * p.x) * std::sin(repeat_factor * p.y) * std::sin(repeat_factor * p.z);
    if (sin < 0) {
        return odd->value(u, v, p);
    }
    return even->value(u, v, p);
}
