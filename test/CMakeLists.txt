add_executable(number_test number_test.cpp)
target_link_libraries(number_test gtest_main arty_math)
add_test(NAME number_test COMMAND number_test)

