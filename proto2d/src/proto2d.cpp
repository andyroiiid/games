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
    SetTitle("テスト");
    SetVSync(true);

#define LOG_TEST(level) Log##level("This is a " #level " log.")
    LOG_TEST(Debug);
    LOG_TEST(Verbose);
    LOG_TEST(Info);
    LOG_TEST(Warning);
    LOG_TEST(Error);
#undef LOG_TEST

#define LOG_GL_STRING(x) LogInfo(#x ": %s\n", glGetString(x))
    LOG_GL_STRING(GL_VERSION);
    LOG_GL_STRING(GL_SHADING_LANGUAGE_VERSION);
    LOG_GL_STRING(GL_VENDOR);
    LOG_GL_STRING(GL_RENDERER);
#undef LOG_GL_STRING
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    m_size = Vec2(size);
    glViewport(0, 0, size.x, size.y);
    m_textRenderer.OnResize(size);
}

void Proto2D::OnTick(float deltaTime) {
#define TEST_KEY(key) do { \
        if (Keyboard::GetKeyDown(Keyboard::key)) { \
            LogInfo(#key " pressed"); \
        } else if (Keyboard::GetKeyUp(Keyboard::key)) { \
            LogInfo(#key " released"); \
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
#undef TEST_KEY

    static const GLfloat clearColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float textHeight = m_textRenderer.GlyphSize().y;

    char buffer[LOG_MAX_CHARS];
    sprintf_s(buffer, "fps = %.2f, size = %.0f x %.0f", 1.0f / deltaTime, m_size.x, m_size.y);
    m_textRenderer.DrawText(buffer, {0, m_size.y - textHeight}, {1, 1, 1, 1});

    IterateLatestLogs([this, textHeight](int i, const LogEntry &log) {
        static constexpr Vec4 colors[5]{
                {0.5f, 1.0f, 0.5f, 1.0f},
                {0.5f, 1.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 0.5f, 1.0f},
                {1.0f, 0.5f, 0.5f, 1.0f},
        };

        const Vec4 &color = colors[static_cast<int>(log.level)];
        m_textRenderer.DrawText(log.message, {0, static_cast<float>(i) * textHeight}, color);
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
