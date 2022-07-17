//
// Created by andyroiiid on 7/16/2022.
//
#include "state/log_viewer.h"

#include <input/keyboard.h>
#include <logging.h>

#include "state/terminal.h"

LogViewer::LogViewer() {
    SetTitle("View Logs");
    LogInfo("constructed LogViewer");
}

LogViewer::~LogViewer() {
    LogInfo("destructed LogViewer");
}

void LogViewer::OnResize(const IntVec2 &size) {
    m_size = Vec2(size);
    glViewport(0, 0, size.x, size.y);
    m_textRenderer.OnResize(size);
}

Proto2D::StateBuilder LogViewer::Update(float deltaTime) {
    m_fps = 1.0f / deltaTime;

    if (Keyboard::GetKeyDown(Keyboard::Tab)) {
        return []() -> std::unique_ptr<Proto2D::State> {
            return std::make_unique<Terminal>();
        };
    } else {
        return nullptr;
    }
}

void LogViewer::Draw() {
    static const GLfloat clearColor[4]{0.4f, 0.4f, 0.4f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float textHeight = m_textRenderer.FontSize().y;

    char buffer[LOG_MAX_CHARS];
    sprintf_s(buffer, "fps = %.2f, size = %.0f x %.0f", m_fps, m_size.x, m_size.y);
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
