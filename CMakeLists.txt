cmake_minimum_required(VERSION 3.20)
project(Bacteria_simulation LANGUAGES CXX VERSION 1.0)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(dependencies)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)

add_library(sources STATIC ${SOURCES} simulator.cpp simulator.h)
set_target_properties(sources PROPERTIES CXX_STANDARD 23)
target_link_libraries(sources PUBLIC ImGui-SFML::ImGui-SFML)

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML sources)
