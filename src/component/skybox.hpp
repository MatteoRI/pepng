#pragma once

#include <pepng.h>

class Skybox : public Component {
    public:
        Skybox(std::shared_ptr<Material> material);
        Skybox(const Skybox& skybox);
        static std::shared_ptr<Skybox> make_skybox(std::shared_ptr<Material> material);
        virtual void render(std::shared_ptr<WithComponents> object) override;
        virtual Skybox* clone_implementation() override;
        virtual void init(std::shared_ptr<WithComponents> parent) override;
        #ifdef IMGUI
        virtual void imgui() override;
        #endif

    private:
        std::shared_ptr<Model> model;
        std::shared_ptr<Material> material;
};

namespace pepng {
    std::shared_ptr<Skybox> make_skybox(std::shared_ptr<Material> material);
}