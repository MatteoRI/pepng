/**
 * This is the standard way for defining new Object cpp for PEPNG.
 */
#include "cube.hpp"

std::shared_ptr<Object> pepng::make_cube(std::shared_ptr<Transform> transform, std::shared_ptr<Texture> texture, GLuint shaderProgram) {
    // Create a base object with name. 
    // Name will be used for IMGUI (and other reflection).
    auto cube = pepng::make_object("Cube");

    // Attach the transform to the object.
    // It is recommended to attach a Transform first.
    cube->attach_component(transform);

    // Attach Renderer.
    cube->attach_component(
        // Create Renderer. Takes Model and Material.
        pepng::make_renderer(
            // Create Model. Basically a collection of buffers.
            pepng::make_model()
                // Attach Buffer.
                ->attach_buffer(
                    // Creates position Buffer.
                    // We need to specify the value type.
                    pepng::make_buffer<glm::vec3>(
                        // The collection of cube triangle points.
                        std::vector {
                            glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),
                            glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),

                            glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), 
                            glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),

                            glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), 
                            glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f),

                            glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f),
                            glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f),

                            glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), 
                            glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),

                            glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), 
                            glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),
                        },
                        // The type of OpenGL buffer. This is usually GL_ARRAY_BUFFER.
                        GL_ARRAY_BUFFER,
                        // The layout index for the GLSL shader.
                        0,
                        // The size of the buffer elements (for example: float = 1, vec2 = 2, vec3 = 3, etc)
                        // In this case, we have a vec3 therefore = 3.
                        3
                    )
                )
                
                // YOU SHOULD INCLUDE A NORMAL BUFFER HERE ASWELL!

                ->attach_buffer(
                    // Creates UV Buffer.
                    pepng::make_buffer<glm::vec2>(
                        // The collection of cube UVs.
                        std::vector {
                            glm::vec2(1.0f), glm::vec2(0.0f), glm::vec2(1.0f, 0.0f),
                            glm::vec2(1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f), 

                            glm::vec2(1.0f), glm::vec2(0.0f), glm::vec2(1.0f, 0.0f),
                            glm::vec2(1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f),

                            glm::vec2(1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f), 
                            glm::vec2(1.0f), glm::vec2(0.0f), glm::vec2(0.0f, 1.0f), 

                            glm::vec2(1.0f, 0.0f), glm::vec2(0.0f), glm::vec2(1.0f),
                            glm::vec2(1.0f), glm::vec2(0.0f), glm::vec2(0.0f, 1.0f),

                            glm::vec2(1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 0.0f),     
                            glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f),

                            glm::vec2(0.0f, 1.0f), glm::vec2(0.0f), glm::vec2(1.0f),
                            glm::vec2(1.0f), glm::vec2(0.0f), glm::vec2(1.0f, 0.0f),
                        },
                        GL_ARRAY_BUFFER,
                        2,
                        2
                    )
                )
                // Sets the number of points in buffer.
                ->set_count(36)
                // Sets the model name.
                ->set_name("Cube"),
            // Create material that contains the shader and texture.
            pepng::make_material(shaderProgram, texture)
        )
    );

    return cube;
}