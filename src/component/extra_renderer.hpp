#pragma once

#include <pepng.h>
#include "extra_material.hpp"

class ExtraRenderer : public Renderer {
    public:
        std::shared_ptr<ExtraMaterial> extra_material;

        static std::shared_ptr<ExtraRenderer> make_extra_renderer(std::shared_ptr<Model> model, std::shared_ptr<ExtraMaterial> material, GLenum render_mode);
        static std::shared_ptr<ExtraRenderer> make_extra_renderer(std::shared_ptr<Renderer> renderer);

        virtual void render(std::shared_ptr<WithComponents> parent) override;

        #ifdef IMGUI
        virtual void imgui() override;
        #endif

    protected:
        virtual ExtraRenderer* clone_implementation() override;

        ExtraRenderer(std::shared_ptr<Model> model, std::shared_ptr<ExtraMaterial> material, GLenum render_mode);
        ExtraRenderer(const ExtraRenderer& renderer);
        ExtraRenderer(const Renderer& renderer);
};

namespace pepng {
    std::shared_ptr<ExtraRenderer> make_extra_renderer(std::shared_ptr<Model> model, std::shared_ptr<ExtraMaterial> material, GLenum render_mode = GL_TRIANGLES);
    std::shared_ptr<ExtraRenderer> make_extra_renderer(std::shared_ptr<Renderer> renderer);
};