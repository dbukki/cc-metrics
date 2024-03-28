cmake_minimum_required(VERSION 3.0)
project(ccexcmp
    VERSION 1.0
    LANGUAGES CXX)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED
    COMPONENTS filesystem) 

include_directories(${Boost_INCLUDE_DIRS})

add_executable(ccexcmp
    source/main.cpp
    source/compare.cpp
    source/parse.cpp
    source/metrics.cpp
    source/snapshot.cpp)

target_link_libraries(ccexcmp
    ${Boost_LIBRARIES})
