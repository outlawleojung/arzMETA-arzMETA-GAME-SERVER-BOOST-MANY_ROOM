cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE src ./src/*.h ./src/*.cpp ./src/*.cc)

add_executable (DummyClient ${src})

target_include_directories(DummyClient PRIVATE ../CoreLib/src ./Include)

target_link_libraries(DummyClient PRIVATE CoreLib)

if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Program Files/gRPC" ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH "C:/Program Files/agones" ${CMAKE_PREFIX_PATH})
endif (WIN32)

find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(DummyClient PRIVATE protobuf::libprotobuf protobuf::libprotoc)

if (UNIX)
    find_package(Protobuf REQUIRED)
    include_directories(${Protobuf_INCLUDE_DIRS})
    target_link_libraries(DummyClient PRIVATE ${Protobuf_LIBRARIES})
endif (UNIX)

if (WIN32)
    find_package(sioclient CONFIG REQUIRED)
    target_link_libraries(DummyClient PRIVATE sioclient::sioclient)
endif (WIN32)