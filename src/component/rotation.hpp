/**
 * This is the standard way for defining new Component hpp for PEPNG.
 * 
 * Every hpp should only contain one Component definition.
 */
#pragma once

#include <pepng.h>

/**
 * Component that rotates Object's Transform by speed * "x", "y" input every frame. 
 */
class Rotation : public Component {
    public:
        /**
         * OO METHODS
         */

        /**
         * Shared_ptr constructor for Rotation.
         * 
         * This method is used over standard constructors as PEPNG standards.
         * 
         * @param speed The rotation speed.
         */
        static std::shared_ptr<Rotation> make_rotation(float speed);

        /**
         * LIFETIME METHODS
         */

        // This method is called when the object is first attached.
        virtual void init(std::shared_ptr<WithComponents> parent) override;

        // This method is called during the update loop.
        virtual void update(std::shared_ptr<WithComponents> parent) override;

        // This method is called during the render loops.
        virtual void render(std::shared_ptr<WithComponents> parent) override;

        /**
         * IMGUI
         * 
         * Given IMGUI is optional, you need to check if it is present.
         */
        #ifdef IMGUI
        virtual void imgui() override;
        #endif
    protected:
        /**
         * CLONE IMPL
         * 
         * We need to supply a raw clone for the shared_ptr.
         */
        virtual Rotation* clone_implementation() override;
    
    private:
        /**
         * CONSTRUCTORS
         * 
         * Constructors are made intentionally private to force usage of shared_ptr constructor.
         */

        // Base constructor.
        Rotation(float speed);
        // Copy constructor. MUST be implemented for all components.
        Rotation(const Rotation& rotation);

        /**
         * MEMBER VARIABLES
         */
        
        // The speed of rotation.
        float __speed;

        // Pointer to Transform. Cached to prevent searching every frame.
        std::shared_ptr<Transform> __transform;
};

// Gives access to shared_ptr constructor to the pepng namespace.
namespace pepng {
    /**
     * Shader_ptr constructor for Rotation.
     * 
     * Uses the inner Rotation contructor.
     */
    std::shared_ptr<Rotation> make_rotation(float speed);
}