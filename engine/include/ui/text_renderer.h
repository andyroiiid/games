//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_TEXT_RENDERER_H
#define GAMES_TEXT_RENDERER_H

#include <array>

#include "gfx/base2d.h"
#include "gfx/texture.h"

class TextRenderer {
public:
    TextRenderer();

    void OnResize(const IntVec2 &size);

    void DrawText(const std::string_view &text, const Vec2 &position, const Vec4 &color);

    [[nodiscard]] const Vec2 &FontSize() const { return m_fontSize; }

private:
    struct InstanceGlyph {
        Vec4 screenRect;
        Vec4 textureRect;

        static void SetupVAO(GLuint vao) {
            SetupVertexArrayFloatsAttrib(vao, 3, 1, 4, offsetof(InstanceGlyph, screenRect));
            SetupVertexArrayFloatsAttrib(vao, 4, 1, 4, offsetof(InstanceGlyph, textureRect));
        }
    };

    using InstancedMeshGlyph = InstancedMesh2D<InstanceGlyph>;

    static constexpr int NUM_CHARS = CHAR_MAX + 1;

    InstancedMeshGlyph m_mesh;
    Shader2D           m_shader;
    GLint              m_colorLocation;

    Texture                     m_texture;
    Vec2                        m_fontSize;
    std::array<Vec4, NUM_CHARS> m_textureGrids;

    Vec2 m_screenScale;
    Vec2 m_fontSizeOnScreen;

    std::vector<InstanceGlyph> m_instances;
};

#endif //GAMES_TEXT_RENDERER_H
