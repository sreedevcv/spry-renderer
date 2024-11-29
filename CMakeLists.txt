cmake_minimum_required(VERSION 3.20.0)
project(Test VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 23)

# add_link_options(-fsanitize=address)
# add_compile_options(-Wall)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DRES_PATH="${CMAKE_SOURCE_DIR}/res/")

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(renderer)

target_link_libraries(
    main
    renderer
)
