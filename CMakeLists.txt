cmake_minimum_required(VERSION 3.22)
project (ctut)

set(CMAKE_CXX_STANDARD 23)

add_executable(ctut main.cpp test_runner.h test_invocable_concept.h test_fixture.h)
