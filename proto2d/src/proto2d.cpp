//
// Created by Andrew Huang on 7/9/2022.
//

#include "proto2d.h"

std::unique_ptr<Game> NewGame() {
    return std::make_unique<Proto2D>();
}

Proto2D::Proto2D() {
#define LOG_GL_STRING(x) fprintf(stderr, #x ": %s\n", glGetString(x))
    LOG_GL_STRING(GL_VERSION);
    LOG_GL_STRING(GL_SHADING_LANGUAGE_VERSION);
    LOG_GL_STRING(GL_VENDOR);
    LOG_GL_STRING(GL_RENDERER);
#undef LOG_GL_STRING

    const Vertex2D vertices[] = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
            {{0.0,   0.5f},  {0.0f, 0.0f, 1.0f, 1.0f}},
    };
    m_mesh = Mesh2D(3, vertices, GL_TRIANGLE_STRIP);

    m_shader = Shader(
            R"GLSL(
#version 450 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec4 aColor;

layout(location = 0) out vec4 vColor;

void main() {
    gl_Position = vec4(aPosition, 0, 1);
    vColor = aColor;
}
)GLSL",
            R"GLSL(
#version 450 core
layout(location = 0) in vec4 vColor;

layout(location = 0) out vec4 fColor;

void main() {
    fColor = vColor;
}
)GLSL"
    );
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    fprintf(stderr, "%d, %d\n", size.x, size.y);
    glViewport(0, 0, size.x, size.y);
}

void Proto2D::OnTick(float deltaTime) {
    const GLfloat clearColor[] = {0.4f, 0.8f, 1.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    m_shader.Use();
    m_mesh.BindAndDraw();
    glBindVertexArray(0);
    glUseProgram(0);
}

void Proto2D::OnClose() {
    RequestQuit();
}
