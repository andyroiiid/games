//
// Created by Andrew Huang on 7/10/2022.
//

#include "gfx/shader.h"

static GLuint createShader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compileStatus = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint infoLogLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto infoLog = new GLchar[infoLogLength];
        glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, infoLog);
        fprintf(stderr, "failed to load shader: %s\n", infoLog);
        delete[] infoLog;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static GLuint createProgram(const std::initializer_list<GLuint> &shaders) {
    GLuint program = glCreateProgram();

    for (GLuint shader: shaders)
        glAttachShader(program, shader);
    glLinkProgram(program);

    GLint linkStatus = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint infoLogLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto infoLog = new GLchar[infoLogLength];
        fprintf(stderr, "failed to link program: %s\n", infoLog);
        delete[] infoLog;
        glDeleteProgram(program);
        return 0;
    }

    for (GLuint shader: shaders)
        glDetachShader(program, shader);
    return program;
}

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) {
    GLuint vertexShader   = createShader(GL_VERTEX_SHADER, vertexSource.c_str());
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
    m_program = createProgram({vertexShader, fragmentShader});
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (m_program) glDeleteProgram(m_program);
}

void Shader::Use() {
    glUseProgram(m_program);
}

GLint Shader::GetUniformLocation(const std::string &name) {
    return glGetUniformLocation(m_program, name.c_str());
}

void Shader::SetUniform(GLint location, float value) {
    glProgramUniform1f(m_program, location, value);
}

void Shader::SetUniform(GLint location, const Vec2 &value) {
    glProgramUniform2fv(m_program, location, 1, reinterpret_cast<const GLfloat *>(&value));
}

void Shader::SetUniform(GLint location, const Vec3 &value) {
    glProgramUniform3fv(m_program, location, 1, reinterpret_cast<const GLfloat *>(&value));
}

void Shader::SetUniform(GLint location, const Vec4 &value) {
    glProgramUniform4fv(m_program, location, 1, reinterpret_cast<const GLfloat *>(&value));
}
