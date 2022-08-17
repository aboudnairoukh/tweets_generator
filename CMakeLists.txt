cmake_minimum_required(VERSION 3.16)
project(ex3a_aboudnairoukh C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ex3a_aboudnairoukh
        linked_list.c
        linked_list.h
        markov_chain.h
        tweets_generator.c
        markov_chain.c ../../AppData/Roaming/JetBrains/CLion2022.1/scratches/ex3a_scratch.c)
