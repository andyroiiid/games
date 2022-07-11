//
// Created by Andrew Huang on 7/11/2022.
//

#include "text_renderer.h"

#include "monogram_font.h"

TextRenderer::TextRenderer() {
    static const std::vector<Vertex2D> vertices{
            {{0, 0}, {0, 0}, {1, 1, 1, 1}},
            {{1, 0}, {1, 0}, {1, 1, 1, 1}},
            {{0, 1}, {0, 1}, {1, 1, 1, 1}},
            {{1, 1}, {1, 1}, {1, 1, 1, 1}},
    };

    m_mesh = Mesh2D(vertices, GL_TRIANGLE_STRIP);

    m_shader = Shader2D(R"GLSL(
layout(location = 0) out vec2 vTexCoord;
layout(location = 1) out vec4 vColor;

layout(location = 0) uniform vec4 uScreenRect;
layout(location = 1) uniform vec4 uTextureRect;

void main() {
    vec2 position = (uScreenRect.xy + uScreenRect.zw * aPosition) * 2 - 1; // map from 0~1 to -1~1
    gl_Position = vec4(position, 0, 1);
    vTexCoord = uTextureRect.xy + uTextureRect.zw * aTexCoord;
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

    m_screenRectLocation  = m_shader.GetUniformLocation("uScreenRect");
    m_textureRectLocation = m_shader.GetUniformLocation("uTextureRect");

    m_texture = Texture(MONOGRAM_FONT_WIDTH, MONOGRAM_FONT_HEIGHT, MONOGRAM_FONT_DATA);

    m_glyphSize = Vec2(static_cast<float>(m_texture.Width()) / 16, static_cast<float>(m_texture.Height()) / 8);
}

void TextRenderer::OnResize(const IntVec2 &size) {
    m_screenScale = 1.0f / Vec2(size);
}

void TextRenderer::DrawText(const std::string &text, float x, float y, float scale) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.Use();
    m_texture.Bind(0);

    for (char c: text) {
        m_shader.SetUniform(m_screenRectLocation, {
                x * m_screenScale.x,
                y * m_screenScale.y,
                m_glyphSize.x * m_screenScale.x * scale,
                m_glyphSize.y * m_screenScale.y * scale,
        });

        // these are hard coded values for 16 x 8 glyphs texture
        m_shader.SetUniform(m_textureRectLocation, {
                static_cast<float>(c % 16) * 0.0625f,
                static_cast<float>(c / 16) * 0.125f, // NOLINT(bugprone-integer-division)
                0.0625f,
                0.125f
        });

        m_mesh.BindAndDraw();

        x += m_glyphSize.x * scale;
    }

    glBindTextureUnit(0, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_BLEND);
}
