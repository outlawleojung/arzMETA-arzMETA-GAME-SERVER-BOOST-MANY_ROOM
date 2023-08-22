cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE src ./src/*.h ./src/*.cpp ./src/*.cc)

add_executable (Server ${src})

target_include_directories(Server PRIVATE ../CoreLib/src ./Include)

target_link_libraries(Server PRIVATE CoreLib)

if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Program Files/gRPC" ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH "C:/Program Files/agones" ${CMAKE_PREFIX_PATH})
endif (WIN32)

if (WIN32)
    target_include_directories(Server PRIVATE "C:/Program Files/OpenSSL-Win64/include")

    target_link_directories(Server PRIVATE "C:/Program Files/OpenSSL-Win64/lib")
    target_link_libraries(Server PRIVATE libssl.lib libcrypto.lib)
endif (WIN32)

find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(Server PRIVATE protobuf::libprotobuf protobuf::libprotoc)

find_package(httplib CONFIG REQUIRED)
target_link_libraries(Server PRIVATE httplib::httplib)

if (WIN32)
    target_include_directories(Server PRIVATE "C:/Program Files/MySQL/MySQL Connector C 6.1/include")
    target_link_directories(Server PRIVATE "C:/Program Files/MySQL/MySQL Connector C 6.1/lib")
endif (WIN32)

if (UNIX)
	target_include_directories(Server PRIVATE "/usr/include/mysql")
endif (UNIX)

find_package(SOCI REQUIRED)
target_link_libraries(Server PRIVATE SOCI::soci_core SOCI::soci_mysql)

#if (WIN32)
#    file(GLOB common_libs ./Lib/Win/*.lib)
#
#    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#        file(GLOB platform_libs ./Lib/Win/Debug/*.lib)
#    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#        file(GLOB platform_libs ./Lib/Win/Release/*.lib)
#    endif()
#
#    target_link_libraries(Server PRIVATE ${common_libs} ${platform_libs})
#
#    find_package(SOCI REQUIRED)
#    
#    target_include_directories(Server PRIVATE "C:/Program Files/MySQL/MySQL Connector C 6.1/include")
#    target_link_directories(Server PRIVATE "C:/Program Files/MySQL/MySQL Connector C 6.1/lib")
#    
#    target_link_libraries(Server PRIVATE SOCI::soci_core SOCI::soci_mysql)
#
#endif (WIN32)
#
#if (UNIX)
#    find_package(Protobuf REQUIRED)
#    target_include_directories(Server PRIVATE ${Protobuf_INCLUDE_DIRS})
#    target_link_libraries(Server PRIVATE ${Protobuf_LIBRARIES})
#
#    target_include_directories(Server PRIVATE "/usr/include/mysql")
#    find_package(SOCI REQUIRED)
#    target_link_libraries(Server PRIVATE ssl crypto)
#    target_link_libraries(Server PRIVATE SOCI::soci_core SOCI::soci_mysql)
#endif (UNIX)
