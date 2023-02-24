cmake_minimum_required(VERSION 3.16)

project(DiscoverEU C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(SFML_DIR /usr/share/SFML/lib/cmake/SFML)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(DiscoverEU ${HEADERS} ${SOURCES})
target_include_directories(DiscoverEU PRIVATE include ${SFML_INCLUDE_DIRS})
target_link_libraries(DiscoverEU ${SFML_LIBRARIES} sfml-graphics)