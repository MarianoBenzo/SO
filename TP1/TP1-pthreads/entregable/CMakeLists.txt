cmake_minimum_required(VERSION 3.12)
project(tp1)

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(test-1 test-1.cpp)
target_link_libraries(test-1 Threads::Threads)

add_executable(test-2 test-2.cpp)
target_link_libraries(test-2 Threads::Threads)

add_executable(test-3 test-3.cpp)
target_link_libraries(test-3 Threads::Threads)