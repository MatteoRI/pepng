#include "extra_renderer.hpp"

ExtraRenderer::ExtraRenderer(std::shared_ptr<Model> model, std::shared_ptr<ExtraMaterial> material, GLenum render_mode) :
    Renderer(model, material, render_mode),
    extra_material(material)
{}

ExtraRenderer::ExtraRenderer(const ExtraRenderer& renderer) :
    Renderer(renderer)
{
    auto material = std::dynamic_pointer_cast<ExtraMaterial>(renderer.extra_material->clone());

    this->extra_material = material;
    this->material = material;
}

ExtraRenderer::ExtraRenderer(const Renderer& renderer) :
    Renderer(renderer)
{
    auto material = pepng::make_extra_material(renderer.material);

    this->extra_material = material;
    this->material = material;
}

ExtraRenderer* ExtraRenderer::clone_implementation() {
    return new ExtraRenderer(*this);
}

void ExtraRenderer::render(std::shared_ptr<WithComponents> parent) {
    auto shaderProgram = this->material->shader_program();

    glUseProgram(shaderProgram);

    GLuint u_has_color = glGetUniformLocation(shaderProgram, "u_has_color");

    if(this->extra_material->color.x >= 0) {
        glUniform1f(u_has_color, GL_TRUE);

        GLuint u_color = glGetUniformLocation(shaderProgram, "u_color");

        glUniform3fv(u_color, 1, glm::value_ptr(this->extra_material->color));
    } else {
        glUniform1f(u_has_color, GL_FALSE);
    }

    Renderer::render(parent);
}

std::shared_ptr<ExtraRenderer> ExtraRenderer::make_extra_renderer(std::shared_ptr<Model> model, std::shared_ptr<ExtraMaterial> material, GLenum render_mode) {
    std::shared_ptr<ExtraRenderer> renderer(new ExtraRenderer(model, material, render_mode));

    return renderer;
}

std::shared_ptr<ExtraRenderer> pepng::make_extra_renderer(std::shared_ptr<Model> model, std::shared_ptr<ExtraMaterial> material, GLenum render_mode) {
    return ExtraRenderer::make_extra_renderer(model, material, render_mode);
}

std::shared_ptr<ExtraRenderer> ExtraRenderer::make_extra_renderer(std::shared_ptr<Renderer> renderer) {
    std::shared_ptr<ExtraRenderer> extra_renderer(new ExtraRenderer(*renderer));

    return extra_renderer;
}

std::shared_ptr<ExtraRenderer> pepng::make_extra_renderer(std::shared_ptr<Renderer> renderer) {
    return ExtraRenderer::make_extra_renderer(renderer);
}

#ifdef IMGUI
void ExtraRenderer::imgui() {
    Renderer::imgui();
}
#endif