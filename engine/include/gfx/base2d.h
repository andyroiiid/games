//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_BASE2D_H
#define GAMES_BASE2D_H

#include "gfx/mesh.h"
#include "gfx/shader.h"

struct Vertex2D {
    Vec2 position;
    Vec2 texCoord;
    Vec4 color;

    static void SetupVAO(GLuint vao) {
        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, position));

        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribBinding(vao, 1, 0);
        glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, texCoord));

        glEnableVertexArrayAttrib(vao, 2);
        glVertexArrayAttribBinding(vao, 2, 0);
        glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, color));
    }
};

using Mesh2D = Mesh<Vertex2D>;

class Shader2D : public Shader {
public:
    Shader2D() = default;

    Shader2D(const std::string &vertexSource, const std::string &fragmentSource) : Shader(
            R"GLSL(
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
)GLSL",
            vertexSource,
            fragmentSource
    ) {}
};

#endif //GAMES_BASE2D_H
