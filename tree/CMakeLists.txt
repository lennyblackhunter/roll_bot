cmake_minimum_required(VERSION 3.15)
project(learning_c__)

set(CMAKE_CXX_STANDARD 17)

add_library(tokenizer
        tokens.cc
        ../sort.hh)

add_library(RPN
        rpn.cc
        rpn.cc
        rpn.hh)

add_executable(tokens_test
        tokens_test.cpp
        tokens.hh
        )

target_link_libraries(tokens_test tokenizer)

add_executable(rpn_test
        rpn_test.cc)

target_link_libraries(rpn_test tokenizer RPN)

add_library(calculator
        calculator.cc
        tokens.hh
        rpn.hh bot.cpp bot.h calculator.hh)

target_link_libraries(calculator tokenizer RPN)

add_executable(bot
    bot.cpp
)

target_link_libraries(bot calculator dpp)
