//
// Created by Andrew Huang on 7/9/2022.
//

#include "proto2d.h"

#include <logging.h>

std::unique_ptr<Game> NewGame() {
    return std::make_unique<Proto2D>();
}

Proto2D::Proto2D() {
    SetTitle("Proto2D");
    m_console.DoFile("assets/test.lua");
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
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
    m_console.ProcessInput();
}

void Proto2D::Draw() {
    static const GLfloat clearColor[4]{0.2f, 0.2f, 0.2f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float textHeight = m_textRenderer.FontSize().y;

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

    glBindVertexArray(0);
    glBindTextureUnit(0, 0);
    glUseProgram(0);

    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_BLEND);
}
