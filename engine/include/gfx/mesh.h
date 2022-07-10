//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_MESH_H
#define GAMES_MESH_H

#include <vector>
#include <glad/gl.h>

#include "movable.h"

template<typename V>
class Mesh {
public:
    Mesh() = default;

    Mesh(Mesh &&) noexcept = default;

    Mesh &operator=(Mesh &&) noexcept = default;

    Mesh(size_t count, const V *data, GLenum mode) {
        m_count = static_cast<GLsizei>(count);
        m_mode  = mode;

        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, count * sizeof(V), data, 0);

        glCreateVertexArrays(1, &m_vao);
        glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(V));

        V::SetupVAO(m_vao);
    }

    Mesh(const std::vector<V> &vertices, GLenum mode)
            : Mesh(vertices.size(), vertices.data(), mode) {}

    ~Mesh() {
        if (m_vbo) glDeleteBuffers(1, &m_vbo);
        if (m_vao) glDeleteVertexArrays(1, &m_vao);
    }

    void BindAndDraw() {
        glBindVertexArray(m_vao);
        glDrawArrays(m_mode, 0, m_count);
    }

protected:
    Movable<GLuint>  m_vbo;
    Movable<GLuint>  m_vao;
    Movable<GLsizei> m_count;
    Movable<GLenum>  m_mode;
};

#endif //GAMES_MESH_H
