//
// Created by Andrew Huang on 7/20/2022.
//

#ifndef GAMES_LUA_CONSOLE_H
#define GAMES_LUA_CONSOLE_H

#include <vector>
#include <string_view>
#include "non_copyable.h"

struct lua_State;

typedef int (*lua_CFunction)(lua_State *L);

class LuaConsole : NonCopyable {
public:
    LuaConsole();

    ~LuaConsole();

    void SetGlobalFunc(const char *name, lua_CFunction function);

    void ProcessInput();

    // only available before next ProcessInput()
    [[nodiscard]] std::string_view CurrentInput() const { return {m_input.data(), m_input.size()}; }

private:
    std::vector<char> m_input;

    lua_State *L = nullptr;
};

#endif //GAMES_LUA_CONSOLE_H
