add_executable(proto2d WIN32
        include/proto2d.h
        include/state/log_viewer.h
        include/state/terminal.h

        src/proto2d.cpp
        src/state/log_viewer.cpp
        src/state/terminal.cpp)

target_include_directories(proto2d PRIVATE include)

target_link_libraries(proto2d PRIVATE engine)