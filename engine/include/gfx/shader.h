//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_SHADER_H
#define GAMES_SHADER_H

#include <string>
#include <glad/gl.h>

#include "movable.h"
#include "math/vector.h"

class Shader {
public:
    Shader() = default;

    Shader(Shader &&) noexcept = default;

    Shader &operator=(Shader &&) noexcept = default;

    Shader(const std::string &vertexInput, const std::string &vertexSource, const std::string &fragmentSource);

    ~Shader();

    void Use();

    GLint GetUniformLocation(const std::string &name);

    void SetUniform(GLint location, float value);

    void SetUniform(GLint location, const Vec2 &value);

    void SetUniform(GLint location, const Vec3 &value);

    void SetUniform(GLint location, const Vec4 &value);

private:
    Movable<GLuint> m_program;
};

#endif //GAMES_SHADER_H
