/**
 * This is the standard way for defining new Object hpp for PEPNG.
 * 
 * Every hpp should only contain one Object definition.
 */
#pragma once

#include <pepng.h>

// New Object definitions should be part of the pepng namespace.
namespace pepng {
    /**
     * Creates a Cube object.
     * 
     * You will typically want to at least pass a Transform and an Object Shader.
     * For more complex models, consider using pepng::load_file.
     * 
     * @param transform The world properties of the object.
     * @param texture The texture to put on the cube faces.
     * @param shaderProgram The object shader program.
     */
    std::shared_ptr<Object> make_cube(std::shared_ptr<Transform> transform, std::shared_ptr<Texture> texture, GLuint shaderProgram);
}