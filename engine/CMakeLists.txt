add_subdirectory(dependencies EXCLUDE_FROM_ALL)

add_library(engine STATIC
        include/engine.h
        include/game.h
        include/system/timer.h
        include/math/vector.h

        src/main.cpp
        src/system/timer.cpp)

target_compile_definitions(engine PUBLIC UNICODE WIN32_LEAN_AND_MEAN)

target_include_directories(engine PUBLIC include)

target_link_libraries(engine PUBLIC Opengl32 glad)