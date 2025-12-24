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
        deque.h
        set.h
        map.h)

list(APPEND MINE_HEADERS
    small_vector.h
    static_vector.h
    ring_buffer.h
    circular_buffer.h
    rope.h
    bitset.h
    dynamic_bitset.h
    unordered_map.h
    unordered_set.h
    flat_map.h
    flat_set.h
    bimap.h
    avl_tree.h
    red_black_tree.h
    treap.h
    skip_list.h
    b_plus_tree.h
    priority_queue.h
    sparse_vector.h
    sparse_set.h
    disjoint_set.h
    segment_tree.h
    fenwick_tree.h
    lru_cache.h
)

target_sources(MineIsBetter PRIVATE ${MINE_HEADERS})
