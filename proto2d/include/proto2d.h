//
// Created by Andrew Huang on 7/9/2022.
//

#ifndef GAMES_PROTO2D_H
#define GAMES_PROTO2D_H

#include <game.h>
#include <input/lua_console.h>
#include <ui/text_renderer.h>

class Proto2D : public Game {
public:
    Proto2D();

    ~Proto2D() override;

    void OnResize(const IntVec2 &size) override;

    void OnTick(float deltaTime) override;

    void OnClose() override;

private:
    void Update(float deltaTime);

    void Draw();

    Vec2 m_size{1.0f, 1.0f};

    Shader2D m_shader;
    GLint    m_cameraPosLocation  = -1;
    GLint    m_cameraSizeLocation = -1;
    GLint    m_spriteRectLocation = -1;

    Mesh2D m_mesh;

    bool         m_consoleActivate = false;
    LuaConsole   m_console;
    TextRenderer m_textRenderer;

    Vec2 m_cameraPos{0.0f, 0.0f};
};

#endif //GAMES_PROTO2D_H
