add_subdirectory(dependencies EXCLUDE_FROM_ALL)

add_library(engine STATIC
        include/non_copyable.h
        include/movable.h
        include/logging.h
        include/game.h
        include/system/timer.h
        include/system/job_system.h
        include/math/vector.h
        include/gfx/mesh.h
        include/gfx/shader.h
        include/gfx/base2d.h
        include/gfx/texture.h

        src/main.cpp
        src/system/timer.cpp
        src/system/job_system.cpp
        src/gfx/shader.cpp
        src/gfx/texture.cpp)

target_compile_definitions(engine PUBLIC UNICODE WIN32_LEAN_AND_MEAN)

target_include_directories(engine PUBLIC include)

target_link_libraries(engine PUBLIC Opengl32 glad)