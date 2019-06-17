add_executable(whole_test whole_test.cpp)
target_link_libraries(whole_test gtest_main arty_math)
add_test(NAME whole_test COMMAND whole_test)

add_executable(integer_test integer_test.cpp)
target_link_libraries(integer_test gtest_main arty_math)
add_test(NAME integer_test COMMAND integer_test)
