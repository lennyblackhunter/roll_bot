# minimum CMake version required
cmake_minimum_required(VERSION 3.15)
# Project name, version and description
project(discord-bot VERSION 1.0 DESCRIPTION "A discord bot")
 
# Add DPP as dependency
# add_subdirectory(libs/DPP)
# add_subdirectory(libs/spdlog) # if you need a logger. Don't forget to clone sources
                              # in the `libs/` directory

# Create an executable
add_executable(${PROJECT_NAME}
    main.cc
    # your others files...
)
 
# Linking libraries
target_link_libraries(${PROJECT_NAME}
    dpp
    #    spdlog # Like before, if you need spdlog
)
 
# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20 # or 20 if you want something more recent
    CXX_STANDARD_REQUIRED ON
)

add_executable(tests
    test.cc
)

add_library(character_sheet
    character_sheet.cc
    character_sheet.hh
        user_input.cc user_input.hh)

target_link_libraries(character_sheet
    fmt calculator
)

target_link_libraries(tests
    character_sheet
)
