cmake_minimum_required(VERSION 3.31)
project(DS_wet2)

set(CMAKE_CXX_STANDARD 14)

add_executable(DS_wet2 main.cpp
        AVLTree.h
        BinaryTree.h
        HashMap.h
        UnionFind.h
)
