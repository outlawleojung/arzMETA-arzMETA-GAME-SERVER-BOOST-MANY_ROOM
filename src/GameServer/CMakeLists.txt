cmake_minimum_required (VERSION 3.8)

file(GLOB _all_proto_files  ${CMAKE_CURRENT_SOURCE_DIR}/Proto/*.proto)
SET(_proto_gen_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/Protocol)
SET(_proto_CSharp_gen_dir ${CMAKE_CURRENT_SOURCE_DIR}/CSharp)

if (WIN32)
    foreach(__proto_file ${_all_proto_files})
        get_filename_component(__current_proto ${__proto_file} NAME)
        execute_process(COMMAND protoc -I=${CMAKE_CURRENT_SOURCE_DIR}/Proto --cpp_out=${_proto_gen_dir} ${__current_proto}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}) 
        execute_process(COMMAND protoc -I=${CMAKE_CURRENT_SOURCE_DIR}/Proto --csharp_out=${_proto_CSharp_gen_dir} ${__current_proto}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}) 
    endforeach(__proto_file)

    execute_process(COMMAND GenPackets.exe --path=${CMAKE_CURRENT_SOURCE_DIR}/Proto --output=${CMAKE_CURRENT_SOURCE_DIR}/src/Room/RoomBase --extension=h --recv=C_ --send=S_ --template RoomBase.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND GenPackets.exe --path=${CMAKE_CURRENT_SOURCE_DIR}/Proto --output=${CMAKE_CURRENT_SOURCE_DIR}/src/ClientPacketHandler --extension=h --recv=C_ --send=S_ --template ClientPacketHandler.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND GenPackets.exe --path=${CMAKE_CURRENT_SOURCE_DIR}/Proto --output=${CMAKE_CURRENT_SOURCE_DIR}/src/ClientPacketHandler --extension=cpp --recv=C_ --send=S_ --template PacketHandler.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    execute_process(COMMAND GenPackets.exe --path=${CMAKE_CURRENT_SOURCE_DIR}/Proto --output=${CMAKE_CURRENT_SOURCE_DIR}/CSharp/PacketManager --extension=cs --recv=C_ --send=S_ --template PacketManager.cs
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif (WIN32)

file(GLOB_RECURSE src ./src/*.h ./src/*.cpp ./src/*.cc)

add_executable (GameServer_Boost ${src})

target_include_directories(GameServer_Boost PRIVATE ../ServerCore/src "./Include") # ServerCore 타겟의 Property 를 이용할 수는 없나?

target_link_libraries(GameServer_Boost PRIVATE ServerCore)

if (WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        file(GLOB libs ./Lib/Win/Debug/*.lib)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        file(GLOB libs ./Lib/Win/Release/*.lib)
    endif()

    target_link_libraries(GameServer_Boost PRIVATE ${libs})
endif (WIN32)

if (UNIX)
    find_package(Protobuf REQUIRED)
    include_directories(${Protobuf_INCLUDE_DIRS})
    target_link_libraries(GameServer_Boost PRIVATE ${Protobuf_LIBRARIES})
endif (UNIX)