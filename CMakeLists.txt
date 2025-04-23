cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Them thu muc include co thu vien Cereal
include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Tim tat ca file .cpp trong src roi them tat ca
file(GLOB_RECURSE SRC_FILE src/*.cpp)
add_executable(main ${SRC_FILE})
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)

# Hi vong la sua duoc loi khong load duoc font
add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/res/ARIAL.TTF
                ${CMAKE_CURRENT_BINARY_DIR}/bin/ARIAL.TTF)
