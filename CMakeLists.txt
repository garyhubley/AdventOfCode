cmake_minimum_required(VERSION 3.10)

# set the project name
project(AdventOfCode2021)

# add the executable
add_executable(AdventOfCode2021)


target_sources(AdventOfCode2021 PUBLIC 
    AdventOfCode2021/adventOfCode.h
    AdventOfCode2021/day01p01.cpp
    AdventOfCode2021/day01p02.cpp
    AdventOfCode2021/day02p01.cpp
    AdventOfCode2021/day02p02.cpp
    AdventOfCode2021/day03p01.cpp
    AdventOfCode2021/day03p02.cpp
    AdventOfCode2021/day04.cpp
    AdventOfCode2021/day05.cpp
    AdventOfCode2021/day06.cpp
    AdventOfCode2021/day07.cpp
    AdventOfCode2021/day08.cpp
    AdventOfCode2021/day09.cpp
    AdventOfCode2021/day10.cpp
    AdventOfCode2021/day11.cpp
    AdventOfCode2021/main.cpp
    AdventOfCode2021/temp.cpp
    )
