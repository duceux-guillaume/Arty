add_executable(number_test number_test.cpp)
target_link_libraries(number_test gtest_main arty)
add_test(NAME number_test COMMAND number_test)

add_executable(matrix_test matrix_test.cpp)
target_link_libraries(matrix_test gtest_main arty)
add_test(NAME matrix_test COMMAND matrix_test)

add_executable(perceptron_test perceptron_test.cpp)
target_link_libraries(perceptron_test gtest_main arty)
add_test(NAME perceptron_test COMMAND perceptron_test)

add_executable(ecs_test ecs_test.cpp)
target_link_libraries(ecs_test gtest_main arty)
add_test(NAME ecs_test COMMAND ecs_test)

add_executable(mat_test test_mat.cpp)
target_link_libraries(mat_test gtest_main arty)
add_test(NAME mat_test COMMAND mat_test)
