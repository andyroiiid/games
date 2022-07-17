//
// Created by Andrew Huang on 7/11/2022.
//

#include "ui/text_renderer.h"

#include "math/grid_layout.h"
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

layout(location = 0) uniform vec4 uColor;

void main() {
    vec2 position = iScreenRect.xy + iScreenRect.zw * aPosition;
    position = position * 2 - 1; // map from 0~1 to -1~1
    gl_Position = vec4(position, 0, 1);
    vTexCoord = iTextureRect.xy + iTextureRect.zw * aTexCoord;
    vColor = aColor * uColor;
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

    m_colorLocation = m_shader.GetUniformLocation("uColor");

    m_texture = Texture(MONOGRAM_FONT_SIZE, MONOGRAM_FONT_DATA);

    GridLayout textureGrids = GridLayout(m_texture.Size(), {16, 8});
    m_fontSize = textureGrids.GetGridSize();
    for (int i = 0; i < NUM_CHARS; i++) {
        m_textureGrids[i] = textureGrids.GetGridRect(i);
    }

    m_instances.reserve(1024);
}

void TextRenderer::OnResize(const IntVec2 &size) {
    m_screenScale      = 1.0f / Vec2(size);
    m_fontSizeOnScreen = FontSize() * m_screenScale;
}

void TextRenderer::DrawText(const std::string_view &text, const Vec2 &position, const Vec4 &color) {
    Vec2 currentPos = position;

    m_instances.clear();
    for (const char c: text) {
        if (isspace(c)) {
            currentPos.x += FontSize().x;
            continue;
        }

        const Vec4 screenRect{currentPos * m_screenScale, m_fontSizeOnScreen};
        m_instances.push_back({screenRect, m_textureGrids[c]});
        currentPos.x += FontSize().x;
    }

    m_shader.Use();
    m_shader.SetUniform(m_colorLocation, color);
    m_texture.Bind(0);
    m_mesh.UpdateInstanceData(m_instances);
    m_mesh.BindAndDrawInstanced();
}
