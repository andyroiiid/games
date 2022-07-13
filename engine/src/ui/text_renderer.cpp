//
// Created by Andrew Huang on 7/11/2022.
//

#include "ui/text_renderer.h"

#include "ui/monogram_font.h"

TextRenderer::TextRenderer() {
    static const std::vector<Vertex2D> vertices{
            {{0, 0}, {0, 0}, {1, 1, 1, 1}},
            {{1, 0}, {1, 0}, {1, 1, 1, 1}},
            {{0, 1}, {0, 1}, {1, 1, 1, 1}},
            {{1, 1}, {1, 1}, {1, 1, 1, 1}},
    };

    m_mesh = InstancedMeshGlyph(vertices, GL_TRIANGLE_STRIP);

    m_shader = Shader2D(R"GLSL(
layout(location = 3) in vec4 iScreenRect;
layout(location = 4) in vec4 iTextureRect;

layout(location = 0) out vec2 vTexCoord;
layout(location = 1) out vec4 vColor;

void main() {
    vec2 position = iScreenRect.xy + iScreenRect.zw * aPosition;
    position = position * 2 - 1; // map from 0~1 to -1~1
    gl_Position = vec4(position, 0, 1);
    vTexCoord = iTextureRect.xy + iTextureRect.zw * aTexCoord;
    vColor = aColor;
}
)GLSL", R"GLSL(
layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec4 vColor;

layout(location = 0) out vec4 fColor;

layout(binding = 0) uniform sampler2D uTexture;

void main() {
    fColor = texture(uTexture, vTexCoord) * vColor;
}
)GLSL");

    m_texture = Texture(MONOGRAM_FONT_WIDTH, MONOGRAM_FONT_HEIGHT, MONOGRAM_FONT_DATA);

    m_glyphSize = Vec2(
            static_cast<float>(m_texture.Width()) * TEXCOORD_PER_GLYPH_X,
            static_cast<float>(m_texture.Height()) * TEXCOORD_PER_GLYPH_Y
    );

    m_instances.reserve(1024);
}

void TextRenderer::OnResize(const IntVec2 &size) {
    m_screenScale = 1.0f / Vec2(size);
}

void TextRenderer::DrawText(const std::string &text, const float x, const float y, float scale) {
    const Vec2 scaledGlyphSize = m_glyphSize * scale;

    float currentX = x;
    m_instances.clear();
    for (char c: text) {
        if (isspace(c)) {
            currentX += scaledGlyphSize.x;
            continue;
        }

        Vec4 screenRect{
                currentX * m_screenScale.x,
                y * m_screenScale.y,
                scaledGlyphSize.x * m_screenScale.x,
                scaledGlyphSize.y * m_screenScale.y
        };
        Vec4 textureRect{
                static_cast<float>(c % NUM_GLYPHS_X) * TEXCOORD_PER_GLYPH_X,
                static_cast<float>(c / NUM_GLYPHS_X) * TEXCOORD_PER_GLYPH_Y, // NOLINT(bugprone-integer-division)
                TEXCOORD_PER_GLYPH_X,
                TEXCOORD_PER_GLYPH_Y
        };
        m_instances.push_back({screenRect, textureRect});
        currentX += scaledGlyphSize.x;
    }

    m_shader.Use();
    m_texture.Bind(0);
    m_mesh.UpdateInstanceData(m_instances);
    m_mesh.BindAndDrawInstanced();
}
