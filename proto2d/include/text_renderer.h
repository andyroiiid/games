//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_TEXT_RENDERER_H
#define GAMES_TEXT_RENDERER_H

#include "gfx/base2d.h"
#include "gfx/texture.h"

class TextRenderer {
public:
    static constexpr float DefaultScale = 2.0f;

    TextRenderer();

    void OnResize(const IntVec2 &size);

    void DrawText(const std::string &text, float x, float y, float scale = DefaultScale);

    [[nodiscard]] Vec2 GlyphSize(float scale = DefaultScale) const { return m_glyphSize * scale; }

private:
    Mesh2D   m_mesh;
    Shader2D m_shader;

    GLint m_screenRectLocation;
    GLint m_textureRectLocation;

    // 16 x 8 glyphs texture (the layout is hard coded)
    Texture m_texture;
    Vec2    m_glyphSize;

    Vec2 m_screenScale;
};

#endif //GAMES_TEXT_RENDERER_H
