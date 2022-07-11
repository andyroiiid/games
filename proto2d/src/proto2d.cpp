//
// Created by Andrew Huang on 7/9/2022.
//

#include "proto2d.h"

#include "logging.h"

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
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    LogInfo("resize %d, %d\n", size.x, size.y);
    glViewport(0, 0, size.x, size.y);
    m_textRenderer.OnResize(size);
}

void Proto2D::OnTick(float deltaTime) {
    const GLfloat clearColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    float textHeight = m_textRenderer.GlyphSize().y;

    ForEachLog([this, textHeight](int i, const std::string &log) {
        m_textRenderer.DrawText(log, 0, static_cast<float>(i) * textHeight);
    });
}

void Proto2D::OnClose() {
    RequestQuit();
}
