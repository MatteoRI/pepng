#include "skybox.hpp"

Skybox::Skybox(std::shared_ptr<Material> material) 
: Component("Skybox"), material(material)
{}

Skybox::Skybox(const Skybox& skybox) 
: Component(skybox), material(skybox.material)
{}

Skybox* Skybox::clone_implementation() {
    return new Skybox(*this);
}

std::shared_ptr<Skybox> Skybox::make_skybox(std::shared_ptr<Material> material) {
    std::shared_ptr<Skybox> skybox(new Skybox(material));

    return skybox;
}

void Skybox::render(std::shared_ptr<WithComponents> parent) {
    if(!this->model->is_init()) {
        this->model->delayed_init();
    }

    if(this->model->vao() == -1 || !this->active()) {
        return;
    }

    auto shaderProgram = this->material->shader_program();

    glUseProgram(shaderProgram);

    if(Camera::current_camera == nullptr) {
        throw std::runtime_error("No current camera set.");
    }

    Camera::current_camera->render(shaderProgram);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, this->material->texture->gl_index());

    std::shared_ptr<Transform> transform;

    try {
        transform = parent->get_component<Transform>();
    } catch(...) {
        std::stringstream ss;

        ss << *parent << " has no transform." << std::endl;

        throw std::runtime_error(ss.str());
    }
    
    GLuint uWorld = glGetUniformLocation(shaderProgram, "u_world");

    if (uWorld >= 0) {
        auto worldMatrix = transform->parent_matrix
            * glm::translate(glm::mat4(1.0f), this->model->offset())
            * transform->world_matrix()
            * glm::translate(glm::mat4(1.0f), -this->model->offset());

        glUniformMatrix4fv(
            uWorld,
            1,
            GL_FALSE,
            glm::value_ptr(worldMatrix)
        );
    }

    GLuint uDisplayTexture = glGetUniformLocation(shaderProgram, "u_display_texture");

    if(uDisplayTexture >= 0) {
        glUniform1f(
            uDisplayTexture,
            GL_TRUE
        );
    }

    glDepthMask(0);

    glBindVertexArray(this->model->vao());

    glDrawArrays(
        GL_TRIANGLES,
        0,
        this->model->count()
    );

    glDepthMask(1);
}

void Skybox::init(std::shared_ptr<WithComponents> object) {
    this->model = pepng::make_model()
    ->attach_buffer(
        pepng::make_buffer<glm::vec3>(
            std::vector {
                glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),

                glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), 
                glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),

                glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), 
                glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f),

                glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f),

                glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), 
                glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),

                glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), 
                glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),
            },
            GL_ARRAY_BUFFER,
            0, 
            3
        )
    )
    ->set_count(36)
    ->set_name("Cube");
}

#ifdef IMGUI
void Skybox::imgui() {
    Component::imgui();
}
#endif

namespace pepng {
    std::shared_ptr<Skybox> make_skybox(std::shared_ptr<Material> material){
        return Skybox::make_skybox(material);
    }
}