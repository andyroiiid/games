add_executable(proto2d WIN32
        include/proto2d.h

        src/proto2d.cpp)

target_include_directories(proto2d PRIVATE include)

target_link_libraries(proto2d PRIVATE engine)