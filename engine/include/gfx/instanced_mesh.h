//
// Created by andyroiiid on 7/11/2022.
//

#ifndef GAMES_INSTANCED_MESH_H
#define GAMES_INSTANCED_MESH_H

#include "gfx/mesh.h"

template<typename V, typename I>
class InstancedMesh : public Mesh<V> {
public:
    InstancedMesh() = default;

    InstancedMesh(InstancedMesh &&) noexcept = default;

    InstancedMesh &operator=(InstancedMesh &&) noexcept = default;

    // dynamic recreate
    InstancedMesh(size_t count, const V *data, GLenum mode)
            : Mesh<V>(count, data, mode) {
        m_instanceCount = 0;

        glCreateBuffers(1, &m_ibo);

        glVertexArrayVertexBuffer(m_vao, 1, m_ibo, 0, sizeof(I));
        glVertexArrayBindingDivisor(m_vao, 1, 1);

        I::SetupVAO(m_vao);
    }

    // dynamic recreate
    InstancedMesh(const std::vector<V> &vertices, GLenum mode)
            : InstancedMesh(vertices.size(), vertices.data(), mode) {}

    // persistent write-only mapping
    InstancedMesh(size_t count, const V *data, GLenum mode, size_t instanceCount)
            : InstancedMesh(count, data, mode) {
        m_instanceCount = static_cast<GLsizei>(instanceCount);

        glNamedBufferStorage(m_ibo, instanceCount * sizeof(I), nullptr, MAPPING_ACCESS);
    }

    // persistent write-only mapping
    InstancedMesh(const std::vector<V> &vertices, GLenum mode, size_t instanceCount)
            : InstancedMesh(vertices.size(), vertices.data(), mode, instanceCount) {}

    // static
    InstancedMesh(size_t count, const V *data, GLenum mode, size_t instanceCount, const I *instanceData)
            : InstancedMesh(count, data, mode) {
        m_instanceCount = static_cast<GLsizei>(instanceCount);

        glNamedBufferStorage(m_ibo, instanceCount * sizeof(I), instanceData, 0);
    }

    // static
    InstancedMesh(const std::vector<V> &vertices, GLenum mode, size_t instanceCount, const I *instanceData)
            : InstancedMesh(vertices.size(), vertices.data(), mode, instanceCount, instanceData) {}

    // static
    InstancedMesh(size_t count, const V *data, GLenum mode, const std::vector<I> &instances)
            : InstancedMesh(count, data, mode, instances.size(), instances.data()) {}

    // static
    InstancedMesh(const std::vector<V> &vertices, GLenum mode, const std::vector<I> &instances)
            : InstancedMesh(vertices.size(), vertices.data(), mode, instances.size(), instances.data()) {}

    ~InstancedMesh() {
        if (m_ibo) glDeleteBuffers(1, &m_ibo);
    }

    void UpdateInstanceData(size_t count, const I *data) {
        m_instanceCount = static_cast<GLsizei>(count);
        glNamedBufferData(m_ibo, count * sizeof(I), data, GL_DYNAMIC_DRAW);
    }

    void UpdateInstanceData(const std::vector<I> &instances) {
        UpdateInstanceData(instances.size(), instances.data());
    }

    // write-only mapping
    void Map() {
        void *data = glMapNamedBufferRange(m_ibo, 0, m_instanceCount * sizeof(I), MAPPING_ACCESS);
        m_data = reinterpret_cast<I *>(data);
    }

    void Unmap() {
        glUnmapNamedBuffer(m_ibo);
        m_data = nullptr;
    }

    I &operator[](size_t idx) {
        return m_data[idx];
    }

    void BindAndDrawInstanced() {
        glBindVertexArray(m_vao);
        glDrawArraysInstanced(m_mode, 0, m_count, m_instanceCount);
    }

private:
    using Mesh<V>::m_vao;
    using Mesh<V>::m_count;
    using Mesh<V>::m_mode;

    static constexpr GLbitfield MAPPING_ACCESS = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;

    Movable<GLuint>  m_ibo;
    Movable<GLsizei> m_instanceCount;
    Movable<I *>     m_data;
};

#endif //GAMES_INSTANCED_MESH_H
