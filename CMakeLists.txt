cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project ("GameServer")

add_subdirectory (src/ServerCore)
add_subdirectory (src/GameServer)

add_dependencies(GameServer_Boost ServerCore)
