/**
 * This is the main file for your program.
 * 
 * Here we define how to setup the scene, how components are attached, and how to bind device inputs.
 */

#include <pepng.h>

// Includes the locally defined components.
#include "./component/components.hpp"
// Includes the locally defined objects.
#include "./object/objects.hpp"

int main() {
    // Attempts to initialize GLFW and IMGUI (if included). If it fails, kill the program by returning -1.
    if (!pepng::init("PEPNG", 1920, 1080)) return -1;

    /**
     * PATHS
     * 
     * Attempts to find absolute path to relative folders.
     * 
     * Each operating system handles folders differently. 
     * Use these methods for finding folders over std::filesystem.
     */
    auto texture_path = pepng::get_folder_path("textures");
    auto model_path = pepng::get_folder_path("models");
    auto shader_path = pepng::get_folder_path("shaders");

    // Binds missing texture. This NEEDS to be performed before loading any other texture.
    pepng::set_missing_texture(texture_path / "missing.jpg");

    // Sets the window icon (optional).
    pepng::set_window_icon(texture_path / "logo.png");

    /**
     * SHADERS
     * 
     * Builds the shaders from GLSL files. 
     * Don't forget that WebGL has limitations (like no Geometry Shader, GLSL ver 300 es, etc).
     * You most likely want to use the EMSCRIPTEN directive to check for WebGL.
     */
    auto object_shader_program = pepng::make_shader_program(
        pepng::make_shader(shader_path / "object" / "vertex.glsl", GL_VERTEX_SHADER),
        pepng::make_shader(shader_path / "object" / "fragment.glsl", GL_FRAGMENT_SHADER)
    );

    // Sets the object shader for the object/scene loader (used later).
    pepng::set_object_shader(object_shader_program);

    /**
     * DEVICES
     * 
     * Binds GLFW input to PEPNG labels.
     * This allows for multiple binding to labels.
     * 
     * The following binds Mouse X, Y and WASD to "x", "y".
     * This is used in /component/rotation.cpp.
     */

    // Creates and attaches Mouse to PEPNG. 
    pepng::attach_device(
        pepng::make_device(MOUSE)
            // Attaches first axis (usually Mouse X) to the "x" label.
            ->attach_unit(pepng::make_axis("x", FIRST))
            // Attaches second axis (usually Mouse Y) to the "y" label.
            ->attach_unit(pepng::make_axis("y", SECOND))
    );

    // Creates and attaches Keyboard to PEPNG.
    pepng::attach_device(
        pepng::make_device(KEYBOARD)
            // Attaches A to positive "x".
            ->attach_unit(pepng::make_button("x", GLFW_KEY_A))
            // Attaches D to negative "x".
            ->attach_unit(pepng::make_button("x", GLFW_KEY_D, -1.0f))
            // Attaches W to positive "y".
            ->attach_unit(pepng::make_button("y", GLFW_KEY_W))
            // Attaches S to negative "y".
            ->attach_unit(pepng::make_button("y", GLFW_KEY_S, -1.0f))
    );

    /**
     * OBJECTS
     * 
     * Instantiates objects to the world.
     * This can be done through files or classes.
     * 
     * (COLLADA files are the most simple and effective.)
     */

    // CUBE

    // Makes Cube defined in /object/cube.
    auto cube = pepng::make_cube(
        // Sets the cube totally centered.
        pepng::make_transform(),
        // Uses the texture at the file location.
        pepng::make_texture(texture_path / "logo.png"),
        // Passing object shader
        object_shader_program
    );

    // Makes and Binds Rotation defined in /component/cube.
    cube->attach_component(pepng::make_rotation(1.0f));

    // Attaches the Cube instance to the world.
    pepng::instantiate(cube);

    // CAMERA

    // Attaches the Camera instance to the world.
    pepng::instantiate(
        // Creates an instance of the Camera component.
        pepng::make_camera_object(
            pepng::make_camera_transform(
                glm::vec3(0.0f, 0.0f, 7.5f),
                glm::vec3(0.0f, 0.0f, 0.0f)
            ),
            pepng::make_camera(
                // Defines position of camera viewport using relative position.
                // Scale is [0, 1] -> the following maps the whole screen.
                pepng::make_viewport(glm::vec2(0.0f), glm::vec2(1.0f)),
                // Defines the perspective.
                // NOTE: aspect ratio is updated during loop, therefore is not important.
                pepng::make_perspective(glm::radians(60.0f), 1, 0.1f, 1000.0f)
            )
        )
    );

    // LOGO

    // Attempts to load model async (defaults to sync for WebGL).
    // COLLADA (better) and OBJ are supported, which can be exported from most 3D modelling program.
    // Calls callbacks inner function when model/scene loaded.
    pepng::load_file(
        // Loading COLLADA file.
        model_path / "logo" / "scene.dae",
        // Callback function that is passed the scene Object (which has the logo as it's child).
        std::function([](std::shared_ptr<Object> object) {
            // Translates the scene up by 2.5 units.
            object->get_component<Transform>()->position += glm::vec3(0.0f, 2.5f, 0.0f);

            // Instantiates the scene (and it's child logo) to the world.
            pepng::instantiate(object);
        }),
        // Translation used for OBJ (set to default for COLLADA).
        pepng::make_transform()
    );

    // Enters the game loop. Returns when the program exits or fails.
    return pepng::update();
}