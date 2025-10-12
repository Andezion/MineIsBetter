cmake_minimum_required(VERSION 3.29)
project(MineIsBetter)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(MineIsBetter
    main.cpp
    vector.h
        stack.h
        array.h
        list.h
        deque.h)
