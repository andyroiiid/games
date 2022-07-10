//
// Created by Andrew Huang on 7/9/2022.
//

#ifndef GAMES_PROTO2D_H
#define GAMES_PROTO2D_H

#include <glad/gl.h>

#include "game.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"

struct Vertex2D {
    Vec2 position;
    Vec4 color;

    static void SetupVAO(GLuint vao) {
        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, position));

        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribBinding(vao, 1, 0);
        glVertexArrayAttribFormat(vao, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, color));
    }
};

using Mesh2D = Mesh<Vertex2D>;

class Proto2D : public Game {
public:
    Proto2D();

    ~Proto2D() override;

    void OnResize(const IntVec2 &size) override;

    void OnTick(float deltaTime) override;

    void OnClose() override;

private:
    Mesh2D m_mesh;
    Shader m_shader;
};

#endif //GAMES_PROTO2D_H
