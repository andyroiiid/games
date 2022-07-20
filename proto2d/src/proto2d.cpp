//
// Created by Andrew Huang on 7/9/2022.
//

#include "proto2d.h"

#include <logging.h>
#include <input/keyboard.h>

std::unique_ptr<Game> NewGame() {
    return std::make_unique<Proto2D>();
}

Proto2D::Proto2D() {
    SetTitle("Console Test");

    m_shader = Shader2D(R"GLSL(
layout(location = 0) out vec2 vTexCoord;
layout(location = 1) out vec4 vColor;

layout(location = 0) uniform vec2 uCameraPos;
layout(location = 1) uniform vec2 uCameraSize;
layout(location = 2) uniform vec4 uSpriteRect;

void main() {
    vec2 worldPosition = uSpriteRect.xy + aPosition * uSpriteRect.zw;
    gl_Position = vec4((worldPosition - uCameraPos) / uCameraSize, 0, 1);
    vTexCoord = aTexCoord;
    vColor = aColor;
}
)GLSL", R"GLSL(
layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec4 vColor;

layout(location = 0) out vec4 fColor;

void main() {
    fColor = vColor;
}
)GLSL");

    m_cameraPosLocation  = m_shader.GetUniformLocation("uCameraPos");
    m_cameraSizeLocation = m_shader.GetUniformLocation("uCameraSize");
    m_spriteRectLocation = m_shader.GetUniformLocation("uSpriteRect");

    static const std::vector<Vertex2D> vertices{
            {{0, 0}, {0, 0}, {1, 1, 1, 1}},
            {{1, 0}, {1, 0}, {1, 1, 1, 1}},
            {{0, 1}, {0, 1}, {1, 1, 1, 1}},
            {{1, 1}, {1, 1}, {1, 1, 1, 1}}
    };
    m_mesh = Mesh2D(vertices, GL_TRIANGLE_STRIP);

    m_console.SetGlobalFunc("quit", [](lua_State *L) {
        RequestQuit();
        return 0;
    });
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    m_size = Vec2(size);

    float halfHeight = 5.0f;
    m_shader.SetUniform(m_cameraSizeLocation, {m_size.x / m_size.y * halfHeight, halfHeight});

    glViewport(0, 0, size.x, size.y);
    m_textRenderer.OnResize(size);
}

void Proto2D::OnTick(float deltaTime) {
    Update(deltaTime);
    Draw();
}

void Proto2D::OnClose() {
    RequestQuit();
}

void Proto2D::Update(float deltaTime) {
    if (Keyboard::GetKeyDown(Keyboard::OemTilde)) {
        if (m_consoleActivate) {
            ClearLogs();
            m_consoleActivate = false;
        } else {
            m_consoleActivate = true;
        }
    }

    if (m_consoleActivate) {
        m_console.ProcessInput();
    } else {
        constexpr float speed = 5.0f;
        if (Keyboard::GetKey(Keyboard::D)) {
            m_cameraPos.x += speed * deltaTime;
        }
        if (Keyboard::GetKey(Keyboard::A)) {
            m_cameraPos.x -= speed * deltaTime;
        }
        if (Keyboard::GetKey(Keyboard::W)) {
            m_cameraPos.y += speed * deltaTime;
        }
        if (Keyboard::GetKey(Keyboard::S)) {
            m_cameraPos.y -= speed * deltaTime;
        }
    }
}

void Proto2D::Draw() {
    static const GLfloat clearColor[4]{0.2f, 0.2f, 0.2f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.Use();
    m_shader.SetUniform(m_cameraPosLocation, m_cameraPos);
    m_shader.SetUniform(m_spriteRectLocation, {-0.5f, -0.5f, 1.0f, 1.0f});
    m_mesh.BindAndDraw();

    const float textHeight = m_textRenderer.FontSize().y;

    char buffer[LOG_MAX_CHARS];
    sprintf_s(buffer, "cameraPos = %.2f, %.2f", m_cameraPos.x, m_cameraPos.y);
    m_textRenderer.DrawText(buffer, {0.0f, m_size.y - textHeight}, {1.0f, 1.0f, 1.0f, 1.0f});

    if (m_consoleActivate) {
        IterateLatestLogs([this, textHeight](int i, const LogEntry &log) {
            static constexpr Vec4 colors[5]{
                    {0.5f, 1.0f, 0.5f, 1.0f},
                    {0.5f, 1.0f, 1.0f, 1.0f},
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    {1.0f, 1.0f, 0.5f, 1.0f},
                    {1.0f, 0.5f, 0.5f, 1.0f},
            };

            const Vec4 &color = colors[static_cast<int>(log.level)];
            m_textRenderer.DrawText(log.message, {0, static_cast<float>(i + 1) * textHeight}, color);
        });

        static const Vec4 inputColor{1.0f, 0.5f, 0.0f, 1.0f};
        m_textRenderer.DrawText(m_console.CurrentInput(), {0, 0}, inputColor);
    }

    glBindVertexArray(0);
    glBindTextureUnit(0, 0);
    glUseProgram(0);

    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_BLEND);
}
