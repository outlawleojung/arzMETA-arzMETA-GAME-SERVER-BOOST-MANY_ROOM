cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE src ./src/*.h ./src/*.cpp)

add_library (CoreLib STATIC ${src})

find_package(Boost COMPONENTS thread random REQUIRED)

target_include_directories(CoreLib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(CoreLib PUBLIC ${Boost_LIBRARIES})