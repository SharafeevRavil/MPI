cmake_minimum_required(VERSION 3.17)
project(MPI)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(MPI
        main.cpp
        tasks/tasks.h
        tasks/task1.cpp
        tasks/task2.cpp
        tasks/task3.cpp
        tasks/task4.cpp
        tasks/task5.cpp
        tasks/task6.cpp
        tasks/task7.cpp
        tasks/task8.cpp
        tasks/task9.cpp
        tasks/task10.cpp
        tasks/task11.cpp
        tasks/task13.cpp
        tasks/finalTask.cpp
        tasks/final86Task.cpp
        )

target_link_libraries(MPI ${MPI_C_LIBRARIES})