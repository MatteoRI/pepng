/**
 * This is the standard way for defining new Component cpp for PEPNG.
 */
#include "rotation.hpp"

/**
 * CONSTRUCTORS
 * 
 * Defined accoriding to C++ standards.
 */

Rotation::Rotation(float speed) :
    // Defines Component name (is required).
    Component("Rotation"),
    __speed(speed)
{}

Rotation::Rotation(const Rotation& rotation) : 
    Component(rotation),
    __speed(rotation.__speed),
    __transform(std::dynamic_pointer_cast<Transform>(rotation.__transform->clone()))
{}

/**
 * BOILERPLATE METHODS
 * 
 * These methods are common to all components.
 * It is recommended to copy the following and modify to your Component needs.
 */

Rotation* Rotation::clone_implementation() {
    return new Rotation(*this);
}

std::shared_ptr<Rotation> Rotation::make_rotation(float speed) {
    std::shared_ptr<Rotation> instance(new Rotation(speed));

    return instance;
}

std::shared_ptr<Rotation> pepng::make_rotation(float speed) {
    return Rotation::make_rotation(speed);
}

/**
 * LIFETIME METHODS
 */

void Rotation::init(std::shared_ptr<WithComponents> parent) {
    // Trys to get Transform component from Object.
    auto transform = parent->get_component<Transform>();

    // Checks if Transform is binded. If fails, prints/throws error.
    if (transform == nullptr) {
        std::stringstream ss;

        ss << *parent << " has no Transform which Rotation requires." << std::endl;

        std::cout << ss.str() << std::endl;

        // NOTE: WASM exceptions kills the engine. YOU WILL NOT BE ABLE TO CATCH!
        throw std::runtime_error(ss.str());
    }

    this->__transform = transform;
}

void Rotation::update(std::shared_ptr<WithComponents> parent) {
    // Using the internal delta_rotate and applies relative rotation.
    // Gets the values of the input "x" and "y" label defined in main.cpp.
    this->__transform->delta_rotate(
        glm::vec3(this->__transform->rotation_matrix() * glm::vec4(
            this->__speed * pepng::input()->axis("x"), 
            this->__speed * pepng::input()->axis("y"),
            0.0f,
            1.0f
        ))
    );
}

// Given we do not use render, we can leave empty.
// It would be better to simply not override this, but was included for clarity.
void Rotation::render(std::shared_ptr<WithComponents> parent) {}

/**
 * IMGUI
 * 
 * We need to check if IMGUI is present.
 */

#ifdef IMGUI
void Rotation::imgui() {
    // Always call the parent imgui first!
    Component::imgui();

    // Gives access to the speed member in the Inspect Window.
    ImGui::InputFloat("Speed", &this->__speed);
}
#endif