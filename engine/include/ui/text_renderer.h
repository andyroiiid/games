//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_TEXT_RENDERER_H
#define GAMES_TEXT_RENDERER_H

#include "gfx/base2d.h"
#include "gfx/texture.h"

class TextRenderer {
public:
    TextRenderer();

    void OnResize(const IntVec2 &size);

    void DrawText(const char *text, const Vec2 &position, const Vec4 &color);

    [[nodiscard]] const Vec2 &GlyphSize() const { return m_glyphSize; }

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

    static constexpr IntVec2 NUM_GRIDS{16, 8};
    static constexpr Vec2    TEXCOORD_PER_GRID = 1.0f / Vec2(NUM_GRIDS);

    InstancedMeshGlyph m_mesh;
    Shader2D           m_shader;
    GLint              m_colorLocation;

    Texture m_texture;
    Vec2    m_glyphSize;
    Vec2    m_screenScale;
    Vec2    m_glyphSizeOnScreen;

    std::vector<InstanceGlyph> m_instances;
};

#endif //GAMES_TEXT_RENDERER_H
