#include "extra_material.hpp"

ExtraMaterial::ExtraMaterial(GLuint shaderProgram, std::shared_ptr<Texture> texture, glm::vec3 color) : 
    Material(shaderProgram, texture),
    color(color)
{}

ExtraMaterial::ExtraMaterial(const ExtraMaterial& material) :
    Material(material),
    color(material.color)
{}

ExtraMaterial::ExtraMaterial(const Material& material, glm::vec3 color) :
    Material(material),
    color(color)
{}

ExtraMaterial* ExtraMaterial::clone_implementation() {
    return new ExtraMaterial(*this);
}

std::shared_ptr<ExtraMaterial> ExtraMaterial::make_extra_material(GLuint shaderProgram, std::shared_ptr<Texture> texture, glm::vec3 color) {
    std::shared_ptr<ExtraMaterial> material(new ExtraMaterial(shaderProgram, texture, color));

    return material;
}

std::shared_ptr<ExtraMaterial> pepng::make_extra_material(GLuint shaderProgram, std::shared_ptr<Texture> texture, glm::vec3 color) {
    return ExtraMaterial::make_extra_material(shaderProgram, texture, color);
}

std::shared_ptr<ExtraMaterial> ExtraMaterial::make_extra_material(std::shared_ptr<Material> material, glm::vec3 color) {
    std::shared_ptr<ExtraMaterial> extra_material(new ExtraMaterial(*material, color));

    return extra_material;
}

std::shared_ptr<ExtraMaterial> pepng::make_extra_material(std::shared_ptr<Material> material, glm::vec3 color) {
    return ExtraMaterial::make_extra_material(material, color);
}