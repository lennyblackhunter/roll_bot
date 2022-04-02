cmake_minimum_required(VERSION 3.15)
project(learning_c__)

set(CMAKE_CXX_STANDARD 20)

# Configure DPP
include_directories(DPP/include)
add_subdirectory(DPP)

include_directories(.)

add_library(tokenizer tokens.cc)

add_library(RPN rpn.cc)

add_executable(tokens_test tokens_test.cpp)

target_link_libraries(tokens_test calculator tokenizer RPN)

add_executable(rpn_test rpn_test.cc)
target_link_libraries(rpn_test tokenizer RPN)

add_library(calculator calculator.cc)

target_link_libraries(calculator tokenizer RPN)

add_library(commands commands.cc)
target_link_libraries(commands cthulhu-user_input cthulhu-character_sheet)

add_executable(bot bot.cpp)
target_link_libraries(bot calculator commands dpp cthulhu-character_sheet)

add_subdirectory(cthulhu)

