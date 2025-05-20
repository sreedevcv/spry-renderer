cmake_minimum_required(VERSION 3.20.0)
project(Test VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 23)

# add_link_options(-fsanitize=address)
add_compile_options(-Wall)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DRES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")

# May need to install `wayland-protocols-devel` and `mesa-libGL-devel` on fedora

add_subdirectory(examples)
add_subdirectory(shader-editor)
add_subdirectory(renderer)

# include_directories("${CMAKE_SOURCE_DIR}/include")

target_link_libraries(
    main
    renderer
)

target_link_libraries(
    editor
    renderer
)