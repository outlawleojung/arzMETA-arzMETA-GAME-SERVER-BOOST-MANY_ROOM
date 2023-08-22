cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE src ./src/*.h ./src/*.cpp)

add_library (CoreLib STATIC ${src})

if (WIN32)
	set(BOOST_ROOT "C:/Boost")
	set(Boost_USE_STATIC_LIBS ON)
endif (WIN32)

find_package(Boost COMPONENTS thread REQUIRED)

target_include_directories(CoreLib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(CoreLib PUBLIC ${Boost_LIBRARIES})