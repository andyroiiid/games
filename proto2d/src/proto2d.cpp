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
    m_jobSystem.QueueJob(std::make_unique<MeshLoadJob>(m_mesh));
    m_jobSystem.QueueJob(std::make_unique<ShaderLoadJob>(m_shader));
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    fprintf(stderr, "resize %d, %d\n", size.x, size.y);
    glViewport(0, 0, size.x, size.y);
}

void Proto2D::OnTick(float deltaTime) {
    m_jobSystem.ProcessFinishedJobs();

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

void Proto2D::MeshLoadJob::Execute() {
    m_vertices = {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{0.5f,  -0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
            {{0.5f,  0.5f},  {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    };
}

void Proto2D::MeshLoadJob::Finish() {
    m_mesh = Mesh2D(m_vertices, GL_TRIANGLE_STRIP);
    (void) m_mesh; // unused
}

void Proto2D::ShaderLoadJob::Execute() {
    m_vertexSource = R"GLSL(
layout(location = 0) out vec4 vColor;

void main() {
    gl_Position = vec4(aPosition, 0, 1);
    vColor = aColor;
}
)GLSL";

    m_fragmentSource = R"GLSL(
layout(location = 0) in vec4 vColor;

layout(location = 0) out vec4 fColor;

void main() {
    fColor = vColor;
}
)GLSL";
}

void Proto2D::ShaderLoadJob::Finish() {
    m_shader = Shader2D(m_vertexSource, m_fragmentSource);
    (void) m_shader; // unused
}
