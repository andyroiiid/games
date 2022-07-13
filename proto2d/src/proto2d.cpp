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
#define LOG_GL_STRING(x) LogInfo(#x ": %s\n", glGetString(x))
    LOG_GL_STRING(GL_VERSION);
    LOG_GL_STRING(GL_SHADING_LANGUAGE_VERSION);
    LOG_GL_STRING(GL_VENDOR);
    LOG_GL_STRING(GL_RENDERER);
#undef LOG_GL_STRING

    SetTitle("测试2D游戏");
    SetVSync(m_vsync);
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    m_size = Vec2(size);
    glViewport(0, 0, size.x, size.y);
    m_textRenderer.OnResize(size);
}

void Proto2D::OnTick(float deltaTime) {
#define TEST_KEY(k) do { \
        if (Keyboard::GetKeyDown(Keyboard::k)) { \
            LogInfo(#k " pressed"); \
        } else if (Keyboard::GetKeyUp(Keyboard::k)) { \
            LogInfo(#k " released"); \
        } \
    } while (false)

    TEST_KEY(W);
    TEST_KEY(A);
    TEST_KEY(S);
    TEST_KEY(D);
    TEST_KEY(LeftShift);
    TEST_KEY(RightShift);
    TEST_KEY(LeftAlt);
    TEST_KEY(RightAlt);
    TEST_KEY(LeftControl);
    TEST_KEY(RightControl);
    TEST_KEY(Space);
    TEST_KEY(LeftWindows);

    if (Keyboard::GetKey(Keyboard::LeftControl) && Keyboard::GetKeyDown(Keyboard::V)) {
        m_vsync = !m_vsync;
        SetVSync(m_vsync);
    }

    char buffer[LOG_MAX_CHARS];
    sprintf_s(buffer, "fps = %.2f, size = %.0f x %.0f", 1.0f / deltaTime, m_size.x, m_size.y);

    static const GLfloat clearColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float textHeight = m_textRenderer.GlyphSize().y;
    m_textRenderer.DrawText(buffer, 0, m_size.y - textHeight);
    ForEachLog([this, textHeight](int i, const std::string &log) {
        m_textRenderer.DrawText(log, 0, static_cast<float>(i) * textHeight);
    });

    glBindVertexArray(0);
    glBindTextureUnit(0, 0);
    glUseProgram(0);

    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_BLEND);
}

void Proto2D::OnClose() {
    RequestQuit();
}
