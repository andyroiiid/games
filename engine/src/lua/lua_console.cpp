//
// Created by Andrew Huang on 7/20/2022.
//

#include "lua/lua_console.h"

#include <lua.hpp>

#include "logging.h"
#include "input/keyboard.h"

static int LuaPrint(lua_State *L) {
    const int n = lua_gettop(L);

    for (int i = 1; i <= n; i++) {
        LogInfo("%s", luaL_tolstring(L, i, nullptr));
    }

    return 0;
}

LuaConsole::LuaConsole() {
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "print", LuaPrint);
}

LuaConsole::~LuaConsole() {
    lua_close(L);
}

void LuaConsole::DoFile(const char *filename) {
    luaL_dofile(L, filename);
}

void LuaConsole::SetGlobalFunc(const char *name, lua_CFunction function) {
    lua_register(L, name, function);
}

struct KeyToChar {
    Keyboard::Key key;
    char          normal;
    char          caps;
    char          shift;
    char          capsAndShift;
};

static const KeyToChar INPUT_MAPPINGS[] = {
        {Keyboard::A,                'a',  'A',  'A', 'a'},
        {Keyboard::B,                'b',  'B',  'B', 'b'},
        {Keyboard::C,                'c',  'C',  'C', 'c'},
        {Keyboard::D,                'd',  'D',  'D', 'd'},
        {Keyboard::E,                'e',  'E',  'E', 'e'},
        {Keyboard::F,                'f',  'F',  'F', 'f'},
        {Keyboard::G,                'g',  'G',  'G', 'g'},
        {Keyboard::H,                'h',  'H',  'H', 'h'},
        {Keyboard::I,                'i',  'I',  'I', 'i'},
        {Keyboard::J,                'j',  'J',  'J', 'j'},
        {Keyboard::K,                'k',  'K',  'K', 'k'},
        {Keyboard::L,                'l',  'L',  'L', 'l'},
        {Keyboard::M,                'm',  'M',  'M', 'm'},
        {Keyboard::N,                'n',  'N',  'N', 'n'},
        {Keyboard::O,                'o',  'O',  'O', 'o'},
        {Keyboard::P,                'p',  'P',  'P', 'p'},
        {Keyboard::Q,                'q',  'Q',  'Q', 'q'},
        {Keyboard::R,                'r',  'R',  'R', 'r'},
        {Keyboard::S,                's',  'S',  'S', 's'},
        {Keyboard::T,                't',  'T',  'T', 't'},
        {Keyboard::U,                'u',  'U',  'U', 'u'},
        {Keyboard::V,                'v',  'V',  'V', 'v'},
        {Keyboard::W,                'w',  'W',  'W', 'w'},
        {Keyboard::X,                'x',  'X',  'X', 'x'},
        {Keyboard::Y,                'y',  'Y',  'Y', 'y'},
        {Keyboard::Z,                'z',  'Z',  'Z', 'z'},

        {Keyboard::Space,            ' ',  ' ',  ' ', ' '},

        {Keyboard::D1,               '1',  '1',  '!', '!'},
        {Keyboard::D2,               '2',  '2',  '@', '@'},
        {Keyboard::D3,               '3',  '3',  '#', '#'},
        {Keyboard::D4,               '4',  '4',  '$', '$'},
        {Keyboard::D5,               '5',  '5',  '%', '%'},
        {Keyboard::D6,               '6',  '6',  '^', '^'},
        {Keyboard::D7,               '7',  '7',  '&', '&'},
        {Keyboard::D8,               '8',  '8',  '*', '*'},
        {Keyboard::D9,               '9',  '9',  '(', '('},
        {Keyboard::D0,               '0',  '0',  ')', ')'},

        {Keyboard::OemSemicolon,     ';',  ';',  ':', ':'},
        {Keyboard::OemPlus,          '=',  '=',  '+', '+'},
        {Keyboard::OemComma,         ',',  ',',  '<', '<'},
        {Keyboard::OemMinus,         '-',  '-',  '_', '_'},
        {Keyboard::OemPeriod,        '.',  '.',  '>', '>'},
        {Keyboard::OemQuestion,      '/',  '/',  '?', '?'},
//        {Keyboard::OemTilde,         '`',  '`',  '~', '~'},

        {Keyboard::OemOpenBrackets,  '[',  '[',  '{', '{'},
        {Keyboard::OemPipe,          '\\', '\\', '|', '|'},
        {Keyboard::OemCloseBrackets, ']',  ']',  '}', '}'},
        {Keyboard::OemQuotes,        '\'', '\'', '"', '"'},
};

void LuaConsole::ProcessInput() {
    const bool caps  = Keyboard::GetCapsLock();
    const bool shift = Keyboard::GetKey(Keyboard::LeftShift) || Keyboard::GetKey(Keyboard::RightShift);

    for (const auto mapping: INPUT_MAPPINGS) {
        if (Keyboard::GetKeyDown(mapping.key)) {
            if (caps) {
                m_input.push_back(shift ? mapping.capsAndShift : mapping.caps);
            } else {
                m_input.push_back(shift ? mapping.shift : mapping.normal);
            }
        }
    }

    if (!m_input.empty()) {
        if (Keyboard::GetKeyDown(Keyboard::Back)) {
            m_input.pop_back();
        }

        if (Keyboard::GetKeyDown(Keyboard::Enter)) {
            m_input.push_back('\0');
            if (luaL_dostring(L, m_input.data()) != LUA_OK) {
                LogError("%s", lua_tostring(L, -1));
            }
            m_input.clear();
        }
    }
}
