cmake_minimum_required(VERSION 3.29)
project(MineIsBetter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MineIsBetter
    main.cpp
    vector.h
        stack.h
        array.h
        algorithm.h
        list.h
        deque.h)
