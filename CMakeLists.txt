cmake_minimum_required(VERSION 3.12)
project(Graphical_Dictionary)

set(CMAKE_CXX_STANDARD 17) # Use C++17

# Find and include raylib
find_package(raylib REQUIRED)

include_directories(${RAYLIB_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link raylib explicitly
add_executable(Graphical_Dictionary ./src/main.cpp ./src/Trie.cpp ./src/UI.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/oxford.txt ${CMAKE_CURRENT_BINARY_DIR}/oxford.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/meaning.txt ${CMAKE_CURRENT_BINARY_DIR}/meaning.txt COPYONLY)

target_link_libraries(Graphical_Dictionary raylib)
