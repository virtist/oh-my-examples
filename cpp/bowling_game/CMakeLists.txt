add_executable(bowling_game_test
    bowling_game.cc
    bowling_game_test.cc
)

target_link_libraries(bowling_game_test GTest::gmock_main)
add_test(AllTests bowling_game_test)