//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_BASE2D_H
#define GAMES_BASE2D_H

#include "gfx/instanced_mesh.h"
#include "gfx/shader.h"

struct Vertex2D {
    Vec2 position;
    Vec2 texCoord;
    Vec4 color;

    static void SetupVAO(GLuint vao) {
        SetupVertexArrayFloatsAttrib(vao, 0, 0, 2, offsetof(Vertex2D, position));
        SetupVertexArrayFloatsAttrib(vao, 1, 0, 2, offsetof(Vertex2D, texCoord));
        SetupVertexArrayFloatsAttrib(vao, 2, 0, 4, offsetof(Vertex2D, color));
    }
};

using Mesh2D = Mesh<Vertex2D>;

template<class I>
using InstancedMesh2D = InstancedMesh<Vertex2D, I>;

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
