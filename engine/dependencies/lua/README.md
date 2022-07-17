# Lua

a simple wrapper for using lua within a cmake project

Lua version: 5.3.5

## Example Usage

CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.11)
project(test_project)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    macro(FetchContent_MakeAvailable NAME)
        FetchContent_GetProperties(${NAME})
        if(NOT ${NAME}_POPULATED)
            FetchContent_Populate(${NAME})
            add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR})
        endif()
    endmacro()
endif()

FetchContent_Declare(lua GIT_REPOSITORY https://github.com/andyroiiid/lua.git)
FetchContent_MakeAvailable(lua)

add_executable(test_project main.cpp)
target_link_libraries(test_project lua)
```

main.cpp
```c++
#include <lua.hpp>

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dostring(L, "print('hello,world')");
    lua_close(L);
    return 0;
}
```
