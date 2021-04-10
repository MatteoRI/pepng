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
#include "./object/axes.hpp"
#include "./object/grid.hpp"
#include "./component/skybox.hpp"
#include "./component/extra_material.hpp"
#include "./component/extra_renderer.hpp"

int main()
{
    // Attempts to initialize GLFW and IMGUI (if included). If it fails, kill the program by returning -1.
    if (!pepng::init("PEPNG", 1920, 1080))
        return -1;

    // Sets the wpr;d background color.
    pepng::set_background_color(glm::vec3(0.0f, 1.0f, 0.0f));

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

    /**
     * TEXTURES
     *  
     * Loading all the necessary textures
    */
    // Binds missing texture. This NEEDS to be performed before loading any other texture.
    pepng::set_missing_texture(texture_path / "missing.jpg");

    // Load screens for stage.
    for (int i = 1; i <= 3; i++)
    {
        pepng::make_texture(model_path / "pa2" / "screens" / (std::to_string(i) + ".jpg"))->delayed_init();
    }

    //Binds the skybox texture
    static auto skybox_texture = pepng::make_texture(texture_path / "skybox2.jpg");
    skybox_texture->delayed_init();

    // Sets the window icon (optional).
    pepng::set_window_icon(texture_path / "logo.png");

    /**
     * SHADERS
     * 
     * Builds the shaders from GLSL files.
     */
    auto object_shader_program = pepng::make_shader_program(
        pepng::make_shader(shader_path / "object" / "vertex.glsl", GL_VERTEX_SHADER),
        pepng::make_shader(shader_path / "object" / "fragment.glsl", GL_FRAGMENT_SHADER));

    // Sets the object shader for the object/scene loader (used later).
    pepng::set_object_shader(object_shader_program);

    auto line_shader_program = pepng::make_shader_program(
        pepng::make_shader(shader_path / "line" / "vertex.glsl", GL_VERTEX_SHADER),
        pepng::make_shader(shader_path / "line" / "fragment.glsl", GL_FRAGMENT_SHADER));

    auto shadow_shader_program = pepng::make_shader_program(
        pepng::make_shader(shader_path / "shadow" / "vertex330.glsl", GL_VERTEX_SHADER),
        pepng::make_shader(shader_path / "shadow" / "fragment330.glsl", GL_FRAGMENT_SHADER),
        pepng::make_shader(shader_path / "shadow" / "geometry.glsl", GL_GEOMETRY_SHADER));

    // Sets the shadow shader for the object/scene loader (used later).
    pepng::set_shadow_shader(shadow_shader_program);

    static auto skybox_shader_program = pepng::make_shader_program(
        pepng::make_shader(shader_path / "skybox" / "vertex.glsl", GL_VERTEX_SHADER),
        pepng::make_shader(shader_path / "skybox" / "fragment.glsl", GL_FRAGMENT_SHADER));

    /**
     * DEVICES
     * 
     * Binds GLFW input to PEPNG labels.
     * This allows for multiple binding to labels.
     * 
     * The following binds Mouse X, Y and WASD to "x", "y".
     * This is used in /component/rotation.cpp.
     */

    // Creates and attaches Mouse.
    auto mouse = pepng::make_device(DeviceType::MOUSE)
                     ->attach_unit(pepng::make_axis("mouseY", AxisType::FIRST))
                     ->attach_unit(pepng::make_axis("mouseX", AxisType::SECOND))
                     ->attach_unit(pepng::make_axis("zoom", AxisType::THIRD, 25.0f, true))
                     ->attach_unit(pepng::make_button("pan", GLFW_MOUSE_BUTTON_MIDDLE))
                     ->attach_unit(pepng::make_button("pan", GLFW_MOUSE_BUTTON_4))
                     ->attach_unit(pepng::make_button("rotate", GLFW_MOUSE_BUTTON_RIGHT));

    pepng::attach_device(mouse);

    auto keyboard = pepng::make_device(DeviceType::KEYBOARD)
                        ->attach_unit(pepng::make_button("vertical", GLFW_KEY_W))
                        ->attach_unit(pepng::make_button("vertical", GLFW_KEY_S, -1.0f))
                        ->attach_unit(pepng::make_button("horizontal", GLFW_KEY_A))
                        ->attach_unit(pepng::make_button("horizontal", GLFW_KEY_D, -1.0f))
                        ->attach_unit(pepng::make_button("svertical", GLFW_KEY_Q))
                        ->attach_unit(pepng::make_button("svertical", GLFW_KEY_E, -1.0f))
                        ->attach_unit(pepng::make_button("shorizontal", GLFW_KEY_C))
                        ->attach_unit(pepng::make_button("shorizontal", GLFW_KEY_V, -1.0f))
                        ->attach_unit(pepng::make_button("yaw", GLFW_KEY_UP))
                        ->attach_unit(pepng::make_button("yaw", GLFW_KEY_DOWN, -1.0f))
                        ->attach_unit(pepng::make_button("pitch", GLFW_KEY_LEFT))
                        ->attach_unit(pepng::make_button("pitch", GLFW_KEY_RIGHT, -1.0f))
                        ->attach_unit(pepng::make_button("triangles", GLFW_KEY_T))
                        ->attach_unit(pepng::make_button("points", GLFW_KEY_P))
                        ->attach_unit(pepng::make_button("lines", GLFW_KEY_L))
                        ->attach_unit(pepng::make_button("recenter", GLFW_KEY_HOME))
                        ->attach_unit(pepng::make_button("shadow", GLFW_KEY_B))
                        ->attach_unit(pepng::make_button("texture", GLFW_KEY_X))
                        ->attach_unit(pepng::make_button("scale", GLFW_KEY_U))
                        ->attach_unit(pepng::make_button("scale", GLFW_KEY_J, -1.0f));

    for (int i = 0; i < 10; i++)
    {
        std::stringstream ss;

        ss << "object_" << i;

        keyboard->attach_unit(pepng::make_button(ss.str(), GLFW_KEY_0 + i));
    }

    pepng::attach_device(keyboard);

    /**
     * OBJECTS
     * 
     * Instantiates objects to the world.
     * This can be done through files or classes.
     * 
     * (COLLADA files are the most simple and effective.)
     */

    // Primitives
    std::shared_ptr<Model> cylinder_model;
    std::shared_ptr<Model> sphere_model;
    std::shared_ptr<Model> cube_model;
    std::shared_ptr<Model> cone_model;

    auto cylinder = pepng::make_model();
    auto sphere = pepng::make_model();
    auto cube = pepng::make_model();
    auto cone = pepng::make_model();

    pepng::extra::load_file_sync(
        model_path / "primitives/cylinder.dae",
        std::function([&](std::shared_ptr<Object> object) {
            auto cylinder = object->children.at(0);
            cylinder_model = cylinder->get_component<Renderer>()->model;
        }),
        pepng::make_transform());

    pepng::extra::load_file_sync(
        model_path / "primitives/sphere.dae",
        std::function([&](std::shared_ptr<Object> object) {
            auto sphere = object->children.at(0);
            sphere_model = sphere->get_component<Renderer>()->model;
        }),
        pepng::make_transform());

    pepng::extra::load_file_sync(
        model_path / "primitives/cube.dae",
        std::function([&](std::shared_ptr<Object> object) {
            auto cube = object->children.at(0);
            cube_model = cube->get_component<Renderer>()->model;
        }),
        pepng::make_transform());

    pepng::extra::load_file_sync(
        model_path / "primitives/cone.dae",
        std::function([&](std::shared_ptr<Object> object) {
            auto cone = object->children.at(0);
            cone_model = cone->get_component<Renderer>()->model;
        }),
        pepng::make_transform());

    // Axis
    auto x_material = pepng::make_extra_material(object_shader_program, pepng::make_texture(texture_path / "texture.jpg"));
    auto y_material = pepng::make_extra_material(object_shader_program, pepng::make_texture(), glm::vec3(0.0f, 1.0f, 0.0f));
    auto z_material = pepng::make_extra_material(object_shader_program, pepng::make_texture(), glm::vec3(0.0f, 0.0f, 1.0f));

    auto x_cylinder = pepng::make_object("x cylinder");
    x_cylinder
        ->attach_component(pepng::make_transform(
            glm::vec3(1.25f, 0.0f, 0.0f),
            glm::vec3(0.0f, 90.0f, 0.0f),
            glm::vec3(0.125f, 0.125f, 1.25f)))
        ->attach_component(pepng::make_extra_renderer(cylinder_model, x_material));

    auto x_cone = pepng::make_object("x cone");
    x_cone
        ->attach_component(pepng::make_transform(
            glm::vec3(0.0f, 0.125f, 1.1f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(1.5f, 0.125f, 2.0f)))
        ->attach_component(pepng::make_extra_renderer(cone_model, x_material));

    auto y_cylinder = pepng::make_object("y cylinder");
    y_cylinder
        ->attach_component(pepng::make_transform(
            glm::vec3(0.0f, 1.25f, 0.0f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.125f, 0.125f, 1.25f)))
        ->attach_component(pepng::make_extra_renderer(cylinder_model, y_material));

    auto y_cone = pepng::make_object("y cone");
    y_cone
        ->attach_component(pepng::make_transform(
            glm::vec3(0.0f, 0.0f, -1.1f),
            glm::vec3(-90.0f, 0.0f, 0.0f),
            glm::vec3(1.5f, 0.125f, 2.0f)))
        ->attach_component(pepng::make_extra_renderer(cone_model, y_material));

    auto z_cylinder = pepng::make_object("z cylinder");
    z_cylinder
        ->attach_component(pepng::make_transform(
            glm::vec3(0.0f, 0.0f, 1.25f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.125f, 0.125f, 1.25f)))
        ->attach_component(pepng::make_extra_renderer(cylinder_model, z_material));

    auto z_cone = pepng::make_object("z cone");
    z_cone
        ->attach_component(pepng::make_transform(
            glm::vec3(0.0f, 0.0f, 1.1f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(1.5f, 0.125f, 2.0f)))
        ->attach_component(pepng::make_extra_renderer(cone_model, z_material));

    auto axis = pepng::make_object("Axes");
    axis->attach_component(pepng::make_transform());
    axis->attach_child(x_cylinder);
    axis->attach_child(y_cylinder);
    axis->attach_child(z_cylinder);
    x_cylinder->attach_child(x_cone);
    y_cylinder->attach_child(y_cone);
    z_cylinder->attach_child(z_cone);

    pepng::instantiate(axis);

    // Letters
    auto letters_material = pepng::make_extra_material(object_shader_program, pepng::make_texture(texture_path / "texture1.jpg"));
    // J
    auto letter_j1 = pepng::make_object("J1");
    letter_j1->attach_component(pepng::make_transform(glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_j2 = pepng::make_object("J2");
    letter_j2->attach_component(pepng::make_transform(glm::vec3(0.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_j3 = pepng::make_object("J3");
    letter_j3->attach_component(pepng::make_transform(glm::vec3(-1.0f, -3.0f, 0.0f),
                                                      glm::vec3(0.0f, 90.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_j = pepng::make_object("J");
    letter_j->attach_component(pepng::make_transform(glm::vec3(0.0f, 10.0f, 0.0f)));
    letter_j->attach_child(letter_j1);
    letter_j->attach_child(letter_j2);
    letter_j->attach_child(letter_j3);

    // A
    auto letter_a1 = pepng::make_object("A1");
    letter_a1->attach_component(pepng::make_transform(glm::vec3(0.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a2 = pepng::make_object("A2");
    letter_a2->attach_component(pepng::make_transform(glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a3 = pepng::make_object("A3");
    letter_a3->attach_component(pepng::make_transform(glm::vec3(-2.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a4 = pepng::make_object("A4");
    letter_a4->attach_component(pepng::make_transform(glm::vec3(-2.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a5 = pepng::make_object("A5");
    letter_a5->attach_component(pepng::make_transform(glm::vec3(-1.0f, -1.0f, 0.0f),
                                                      glm::vec3(0.0f, 90.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_a6 = pepng::make_object("A6");
    letter_a6->attach_component(pepng::make_transform(glm::vec3(-1.0f, 1.0f, 0.0f),
                                                      glm::vec3(0.0f, 90.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_a = pepng::make_object("A");
    letter_a->attach_component(pepng::make_transform(glm::vec3(0.0f, 15.0f, 0.0f)));
    letter_a->attach_child(letter_a1);
    letter_a->attach_child(letter_a2);
    letter_a->attach_child(letter_a3);
    letter_a->attach_child(letter_a4);
    letter_a->attach_child(letter_a5);
    letter_a->attach_child(letter_a6);

    // H
    auto letter_h1 = pepng::make_object("H1");
    letter_h1->attach_component(pepng::make_transform(glm::vec3(0.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_h2 = pepng::make_object("H2");
    letter_h2->attach_component(pepng::make_transform(glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_h3 = pepng::make_object("H3");
    letter_h3->attach_component(pepng::make_transform(glm::vec3(-2.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_h4 = pepng::make_object("H4");
    letter_h4->attach_component(pepng::make_transform(glm::vec3(-2.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_h5 = pepng::make_object("H5");
    letter_h5->attach_component(pepng::make_transform(glm::vec3(-1.0f, -1.0f, 0.0f),
                                                      glm::vec3(0.0f, 90.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_h = pepng::make_object("H");
    letter_h->attach_component(pepng::make_transform(glm::vec3(0.0f, 20.0f, 0.0f)));
    letter_h->attach_child(letter_h1);
    letter_h->attach_child(letter_h2);
    letter_h->attach_child(letter_h3);
    letter_h->attach_child(letter_h4);
    letter_h->attach_child(letter_h5);

    // A2
    auto letter_a21 = pepng::make_object("A21");
    letter_a21->attach_component(pepng::make_transform(glm::vec3(0.0f, -2.0f, 0.0f),
                                                       glm::vec3(90.0f, 0.0f, 0.0f),
                                                       glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a22 = pepng::make_object("A22");
    letter_a22->attach_component(pepng::make_transform(glm::vec3(0.0f, 0.0f, 0.0f),
                                                       glm::vec3(90.0f, 0.0f, 0.0f),
                                                       glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a23 = pepng::make_object("A23");
    letter_a23->attach_component(pepng::make_transform(glm::vec3(-2.0f, -2.0f, 0.0f),
                                                       glm::vec3(90.0f, 0.0f, 0.0f),
                                                       glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a24 = pepng::make_object("A24");
    letter_a24->attach_component(pepng::make_transform(glm::vec3(-2.0f, 0.0f, 0.0f),
                                                       glm::vec3(90.0f, 0.0f, 0.0f),
                                                       glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_a25 = pepng::make_object("A25");
    letter_a25->attach_component(pepng::make_transform(glm::vec3(-1.0f, -1.0f, 0.0f),
                                                       glm::vec3(0.0f, 90.0f, 0.0f),
                                                       glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_a26 = pepng::make_object("A26");
    letter_a26->attach_component(pepng::make_transform(glm::vec3(-1.0f, 1.0f, 0.0f),
                                                       glm::vec3(0.0f, 90.0f, 0.0f),
                                                       glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_2a = pepng::make_object("2A");
    letter_2a->attach_component(pepng::make_transform(glm::vec3(0.0f, 25.0f, 0.0f)));
    letter_2a->attach_child(letter_a21);
    letter_2a->attach_child(letter_a22);
    letter_2a->attach_child(letter_a23);
    letter_2a->attach_child(letter_a24);
    letter_2a->attach_child(letter_a25);
    letter_2a->attach_child(letter_a26);

    // N
    auto letter_n1 = pepng::make_object("N1");
    letter_n1->attach_component(pepng::make_transform(glm::vec3(0.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_n2 = pepng::make_object("N2");
    letter_n2->attach_component(pepng::make_transform(glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_n3 = pepng::make_object("N3");
    letter_n3->attach_component(pepng::make_transform(glm::vec3(-2.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_n4 = pepng::make_object("N4");
    letter_n4->attach_component(pepng::make_transform(glm::vec3(-2.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_n5 = pepng::make_object("N5");
    letter_n5->attach_component(pepng::make_transform(glm::vec3(-1.0f, -1.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 30.0f),
                                                      glm::vec3(0.25f, 0.25f, 2.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_n = pepng::make_object("N");
    letter_n->attach_component(pepng::make_transform(glm::vec3(0.0f, 30.0f, 0.0f)));
    letter_n->attach_child(letter_n1);
    letter_n->attach_child(letter_n2);
    letter_n->attach_child(letter_n3);
    letter_n->attach_child(letter_n4);
    letter_n->attach_child(letter_n5);

    // P
    auto letter_p1 = pepng::make_object("P1");
    letter_p1->attach_component(pepng::make_transform(glm::vec3(-2.0f, -2.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_p2 = pepng::make_object("P2");
    letter_p2->attach_component(pepng::make_transform(glm::vec3(-2.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_p3 = pepng::make_object("P3");
    letter_p3->attach_component(pepng::make_transform(glm::vec3(-1.0f, 1.0f, 0.0f),
                                                      glm::vec3(0.0f, 90.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_p4 = pepng::make_object("P4");
    letter_p4->attach_component(pepng::make_transform(glm::vec3(0.0f, 0.0f, 0.0f),
                                                      glm::vec3(90.0f, 0.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(sphere_model, letters_material));

    auto letter_p5 = pepng::make_object("P5");
    letter_p5->attach_component(pepng::make_transform(glm::vec3(-1.0f, -1.0f, 0.0f),
                                                      glm::vec3(0.0f, 90.0f, 0.0f),
                                                      glm::vec3(0.25f, 0.25f, 1.0f)))
        ->attach_component(pepng::make_extra_renderer(cube_model, letters_material));

    auto letter_p = pepng::make_object("P");
    letter_p->attach_component(pepng::make_transform(glm::vec3(0.0f, 35.0f, 0.0f)));
    letter_p->attach_child(letter_p1);
    letter_p->attach_child(letter_p2);
    letter_p->attach_child(letter_p3);
    letter_p->attach_child(letter_p4);
    letter_p->attach_child(letter_p5);

    auto letters = pepng::make_object("Letters");
    letters->attach_component(pepng::make_transform(glm::vec3(0.0f, -5.0f, -20.0f)));
    letters->attach_child(letter_j);
    letters->attach_child(letter_a);
    letters->attach_child(letter_h);
    letters->attach_child(letter_2a);
    letters->attach_child(letter_n);
    letters->attach_child(letter_p);
    pepng::instantiate(letters);

    // PA2 scne
    pepng::instantiate(
        pepng::make_grid(
            pepng::make_transform(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(128.0f, 128.0f, 128.0f)),
            line_shader_program,
            129, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    pepng::load_file(
        model_path / "pa2" / "scene.dae",
        std::function([](std::shared_ptr<Object> object) {
            object->attach_component(pepng::make_selector());

            object->get_component<Transform>()->position = glm::vec3(0.0f, 0.0f, -25.0f);

            // Binds components to loaded objects.
            object->for_each([](std::shared_ptr<Object> obj) {
                obj->attach_component(pepng::make_transformer());

                if (obj->has_component<Renderer>())
                {
                    auto extra_renderer = pepng::make_extra_renderer(obj->get_component<Renderer>());
                    obj->replace_components<Renderer>(extra_renderer);
                }

                // Adds DynamicTexture if object named Display (in this case, the screen).
                if (obj->name == "Display")
                {
                    obj->attach_component(pepng::make_dynamic_texture(2, 4));
                }
            });

            pepng::instantiate(object);
        }),
        pepng::make_transform());

    // SKYBOX
    auto skybox = pepng::make_object("Skybox");
    skybox->attach_component(pepng::make_transform(glm::vec3(0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(100.0f)))
        ->attach_component(pepng::make_skybox(pepng::make_material(skybox_shader_program, skybox_texture)));

    pepng::instantiate(skybox);

    // CAMERA
    // Attaches the Camera instance to the world and creates an instance of the Camera component.
    auto camera =
        pepng::make_camera_object(
            pepng::make_camera_transform(
                glm::vec3(0.0f, 12.5f, 50.0f),
                glm::vec3(0.0f, 0.0f, 0.0f)),
            pepng::make_camera(
                // Defines position of camera viewport using relative position.
                // Scale is [0, 1] -> the following maps the whole screen.
                pepng::make_viewport(glm::vec2(0.0f), glm::vec2(1.0f)),
                // Defines the perspective.
                // NOTE: aspect ratio is updated during loop, therefore is not important.
                pepng::make_perspective(glm::radians(60.0f), 1, 0.1f, 1000.0f)));
    //Adds FPS controller to the camera.
    camera->attach_component(pepng::make_fps());

    //Instantiates the camera.
    pepng::instantiate(camera);

    // Enters the game loop. Returns when the program exits or fails.
    return pepng::update();
}