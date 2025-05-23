cmake_minimum_required(VERSION 3.28)
project(imgui-sfml
  LANGUAGES CXX
  VERSION 1.0
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Them thu muc include co thu vien Cereal
include_directories(${CMAKE_SOURCE_DIR}/include)

# Them cac thu vien sfml, imgui, imgui-sfml
add_subdirectory(dependencies)

# Tim tat ca file .cpp trong src roi them tat ca
file(GLOB_RECURSE SRC_FILE src/*.cpp)
add_executable(main ${SRC_FILE})

target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML)

# Hi vong la sua duoc loi khong load duoc font
add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/res/ARIAL.TTF
                ${CMAKE_CURRENT_BINARY_DIR}/bin/ARIAL.TTF)