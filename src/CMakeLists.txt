file(GLOB LIB_SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
file(GLOB UI_FILES "*.ui")

list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_library(typegame_lib STATIC
    ${LIB_SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

target_include_directories(typegame_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(typegame_lib
    Qt5::Core
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::Network
    Qt5::Test
)

add_executable(typegame
    main.cpp
    ${HEADERS}
    ${RES_MAIN}
)

if(TARGET Qt5::rcc)
    set(RCC_TOOL Qt5::rcc)
elseif(DEFINED Qt5Core_RCC_EXECUTABLE)
    set(RCC_TOOL "${Qt5Core_RCC_EXECUTABLE}")
else()
    find_program(RCC_TOOL rcc REQUIRED)
endif()

add_custom_command(TARGET typegame POST_BUILD
    COMMAND ${RCC_TOOL} --binary ${RES_GAME} -o "$<TARGET_FILE_DIR:typegame>/res_game.rcc"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:typegame>/data"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/data/space_highscores.txt"
        "$<TARGET_FILE_DIR:typegame>/data/space_highscores.txt"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/data/word.txt"
        "$<TARGET_FILE_DIR:typegame>/data/word.txt"
    COMMENT "Compiling game resources into res_game.rcc and copying data files"
)

target_link_libraries(typegame
    typegame_lib
)

if(WIN32 AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(typegame PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

set_target_properties(typegame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
