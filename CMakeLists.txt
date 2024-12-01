cmake_minimum_required(VERSION 3.29)
project(spray_paint)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_library(spray_paint_lib
        src/huffman.cpp
        src/huffman.h
        src/heap/heap.h
        src/heap/min_heap.h
        src/heap/max_heap.h
)
target_link_libraries(spray_paint_lib ${Boost_LIBRARIES})
add_executable(spray_paint_test
        tests/sp_test.cpp
)
target_link_libraries(spray_paint_test
        GTest::gtest_main
        spray_paint_lib
)

add_executable(spray_paint main.cpp)
target_link_libraries(spray_paint spray_paint_lib)

include(GoogleTest)
gtest_discover_tests(spray_paint_test)