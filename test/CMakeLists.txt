add_executable(ecs_test ecs_test.cpp)
target_link_libraries(ecs_test gtest_main arty)
add_test(NAME ecs_test COMMAND ecs_test)

add_executable(mat_test test_mat.cpp)
target_link_libraries(mat_test gtest_main arty)
add_test(NAME mat_test COMMAND mat_test)
