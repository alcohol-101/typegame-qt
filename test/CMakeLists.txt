function(add_qt_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME}
        ${TEST_SOURCE}
        ${TYPEGAME_RESOURCE}
    )
    target_link_libraries(${TEST_NAME}
        typegame_lib
        Qt5::Core
        Qt5::Widgets
        Qt5::Test
        Qt5::Multimedia
        Qt5::Network
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_qt_test(tst_buttonwithtext tst_buttonwithtext.cpp)
add_qt_test(tst_tristatebutton tst_tristatebutton.cpp)
add_qt_test(tst_nameinputdialog tst_nameinputdialog.cpp)
add_qt_test(tst_highscoredialog tst_highscoredialog.cpp)
add_qt_test(tst_exitconfirmdialog tst_exitconfirmdialog.cpp)
add_qt_test(tst_applelogic tst_applelogic.cpp)
add_qt_test(tst_spacelogic tst_spacelogic.cpp)
add_qt_test(tst_mainwindow tst_mainwindow.cpp)
