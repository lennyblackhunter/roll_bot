add_library(cthulhu-character_sheet character_sheet.cc)
target_link_libraries(cthulhu-character_sheet calculator)

add_library(cthulhu-user_input user_input.cc)
target_link_libraries(cthulhu-user_input cthulhu-character_sheet)

add_executable(cthulhu-tests test.cc)
target_link_libraries(cthulhu-tests cthulhu-character_sheet)

add_executable(make_characters make_characters.cc)
target_link_libraries(make_characters config cthulhu-character_sheet)
