//
// Created by Andrew Huang on 7/9/2022.
//

#ifndef GAMES_PROTO2D_H
#define GAMES_PROTO2D_H

#include <game.h>
#include <lua/lua_console.h>
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

    LuaConsole   m_console;
    TextRenderer m_textRenderer;
};

#endif //GAMES_PROTO2D_H
