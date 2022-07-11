//
// Created by Andrew Huang on 7/9/2022.
//

#include "proto2d.h"

#include "logging.h"
#include "monogram_font.h"

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
    m_jobSystem.QueueJob(std::make_unique<MeshLoadJob>(m_mesh));
    m_jobSystem.QueueJob(std::make_unique<ShaderLoadJob>(m_shader));
    m_jobSystem.QueueJob(std::make_unique<TextureLoadJob>(m_texture));
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    LogInfo("resize %d, %d\n", size.x, size.y);
    glViewport(0, 0, size.x, size.y);
}

void Proto2D::OnTick(float deltaTime) {
    m_jobSystem.ProcessFinishedJobs();

    const GLfloat clearColor[] = {0.4f, 0.8f, 1.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.Use();
    m_texture.Bind(0);
    m_mesh.BindAndDraw();
    glBindTextureUnit(0, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_BLEND);
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
layout(location = 0) out vec2 vTexCoord;
layout(location = 1) out vec4 vColor;

void main() {
    gl_Position = vec4(aPosition, 0, 1);
    vTexCoord = aTexCoord;
    vColor = aColor;
}
)GLSL";

    m_fragmentSource = R"GLSL(
layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec4 vColor;

layout(location = 0) out vec4 fColor;

layout(binding = 0) uniform sampler2D uTexture;

void main() {
    fColor = texture(uTexture, vTexCoord) * vColor;
}
)GLSL";
}

void Proto2D::ShaderLoadJob::Finish() {
    m_shader = Shader2D(m_vertexSource, m_fragmentSource);
    (void) m_shader; // unused
}

void Proto2D::TextureLoadJob::Execute() {
}

void Proto2D::TextureLoadJob::Finish() {
    m_texture = Texture(MONOGRAM_FONT_WIDTH, MONOGRAM_FONT_HEIGHT, MONOGRAM_FONT_DATA);
    (void) m_texture;
}
