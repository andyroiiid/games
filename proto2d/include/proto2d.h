//
// Created by Andrew Huang on 7/9/2022.
//

#ifndef GAMES_PROTO2D_H
#define GAMES_PROTO2D_H

#include "game.h"
#include "system/job_system.h"
#include "gfx/base2d.h"

class Proto2D : public Game {
public:
    Proto2D();

    ~Proto2D() override;

    void OnResize(const IntVec2 &size) override;

    void OnTick(float deltaTime) override;

    void OnClose() override;

private:
    Mesh2D   m_mesh;
    Shader2D m_shader;

    class MeshLoadJob : public Job {
    public:
        explicit MeshLoadJob(Mesh2D &mesh) : m_mesh(mesh) {}

        void Execute() override;

        void Finish() override;

    private:
        std::vector<Vertex2D> m_vertices;

        Mesh2D &m_mesh;
    };

    class ShaderLoadJob : public Job {
    public:
        explicit ShaderLoadJob(Shader2D &shader) : m_shader(shader) {}

        void Execute() override;

        void Finish() override;

    private:
        std::string m_vertexSource;
        std::string m_fragmentSource;

        Shader2D &m_shader;
    };

    // job system will destruct before resources
    // so it's safe to store references in jobs
    JobSystem m_jobSystem;
};

#endif //GAMES_PROTO2D_H
