#pragma once

#include <pepng.h>

namespace pepng {
    /**
     * Creates an Axes object.
     */
    std::shared_ptr<Object> make_axes(std::shared_ptr<Transform> transform, GLuint shaderProgram);
}