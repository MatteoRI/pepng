#pragma once

#include <pepng.h>

class ExtraMaterial : public Material {
    public:
        glm::vec3 color;

        static std::shared_ptr<ExtraMaterial> make_extra_material(GLuint shaderProgram, std::shared_ptr<Texture> texture, glm::vec3 color);

        static std::shared_ptr<ExtraMaterial> make_extra_material(std::shared_ptr<Material> material, glm::vec3 color);

    protected:
        virtual ExtraMaterial* clone_implementation() override;

        ExtraMaterial(GLuint shaderProgram, std::shared_ptr<Texture> texture, glm::vec3 color);
        ExtraMaterial(const ExtraMaterial& material);
        ExtraMaterial(const Material& material, glm::vec3 color);
};

namespace pepng {
    std::shared_ptr<ExtraMaterial> make_extra_material(GLuint shaderProgram, std::shared_ptr<Texture> texture, glm::vec3 color = glm::vec3(-1.0f));

    std::shared_ptr<ExtraMaterial> make_extra_material(std::shared_ptr<Material> material, glm::vec3 color = glm::vec3(-1.0f));
};