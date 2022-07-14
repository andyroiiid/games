add_subdirectory(dependencies EXCLUDE_FROM_ALL)

add_library(engine STATIC
        include/non_copyable.h
        include/movable.h
        include/singleton.h
        include/logging.h
        include/game.h
        include/system/timer.h
        include/system/job_system.h
        include/input/keyboard.h
        include/math/vector.h
        include/gfx/mesh.h
        include/gfx/instanced_mesh.h
        include/gfx/shader.h
        include/gfx/base2d.h
        include/gfx/texture.h
        include/ui/monogram_font.h
        include/ui/text_renderer.h

        src/logging.cpp
        src/main.cpp
        src/system/timer.cpp
        src/system/job_system.cpp
        src/input/keyboard.cpp
        src/gfx/shader.cpp
        src/gfx/texture.cpp
        src/ui/text_renderer.cpp)

target_compile_definitions(engine PUBLIC UNICODE WIN32_LEAN_AND_MEAN)

target_include_directories(engine PUBLIC include)

target_link_libraries(engine PUBLIC Imm32 Opengl32 glad)