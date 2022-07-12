//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_TEXT_RENDERER_H
#define GAMES_TEXT_RENDERER_H

#include "gfx/base2d.h"
#include "gfx/texture.h"

class TextRenderer {
public:
    static constexpr float DefaultScale = 1.0f;

    TextRenderer();

    void OnResize(const IntVec2 &size);

    void DrawText(const std::string &text, float x, float y, float scale = DefaultScale);

    [[nodiscard]] Vec2 GlyphSize(float scale = DefaultScale) const { return m_glyphSize * scale; }

private:
    struct InstanceGlyph {
        Vec4 screenRect;
        Vec4 textureRect;

        static void SetupVAO(GLuint vao) {
            glEnableVertexArrayAttrib(vao, 3);
            glVertexArrayAttribBinding(vao, 3, 1);
            glVertexArrayAttribFormat(vao, 3, 4, GL_FLOAT, GL_FALSE, offsetof(InstanceGlyph, screenRect));

            glEnableVertexArrayAttrib(vao, 4);
            glVertexArrayAttribBinding(vao, 4, 1);
            glVertexArrayAttribFormat(vao, 4, 4, GL_FLOAT, GL_FALSE, offsetof(InstanceGlyph, textureRect));
        }
    };

    using InstancedMeshGlyph = InstancedMesh2D<InstanceGlyph>;

    static constexpr int   NUM_GLYPHS_X         = 16;
    static constexpr int   NUM_GLYPHS_Y         = 8;
    static constexpr float TEXCOORD_PER_GLYPH_X = 1.0f / NUM_GLYPHS_X;
    static constexpr float TEXCOORD_PER_GLYPH_Y = 1.0f / NUM_GLYPHS_Y;

    InstancedMeshGlyph m_mesh;
    Shader2D           m_shader;

    Texture m_texture;
    Vec2    m_glyphSize;

    Vec2 m_screenScale;

    std::vector<InstanceGlyph> m_instances;
};

#endif //GAMES_TEXT_RENDERER_H
