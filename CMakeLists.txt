cmake_minimum_required(VERSION 3.0)
project(cc-metrics
    VERSION 1.0
    LANGUAGES CXX)


set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED
    COMPONENTS filesystem) 

include_directories(${Boost_INCLUDE_DIRS})


add_executable(ccexcmp
    source/cmp_main.cpp
    source/compare.cpp
    source/parse.cpp
    source/codecache.cpp
    source/metrics.cpp
    source/snapshot.cpp)

target_link_libraries(ccexcmp
    ${Boost_LIBRARIES})


add_executable(ccexacc
    source/acc_main.cpp
    source/accumulate.cpp
    source/parse.cpp)

target_link_libraries(ccexacc
    ${Boost_LIBRARIES})

