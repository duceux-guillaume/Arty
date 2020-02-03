add_executable(math_test math_test.cpp)
target_link_libraries(math_test gtest_main arty_core)
add_test(NAME math_test COMMAND math_test)

add_executable(memory_test memory_test.cpp)
target_link_libraries(memory_test gtest_main arty_core)
add_test(NAME memory_test COMMAND memory_test)

add_executable(physics_test physics_test.cpp)
target_link_libraries(physics_test gtest_main arty_core)
add_test(NAME physics_test COMMAND physics_test)

add_executable(geometry_test geometry_test.cpp)
target_link_libraries(geometry_test gtest_main arty_core)
add_test(NAME geometry_test COMMAND geometry_test)

add_executable(camera_test camera_test.cpp)
target_link_libraries(camera_test gtest_main arty_core)
add_test(NAME camera_test COMMAND camera_test)

add_executable(number_test number_test.cpp)
target_link_libraries(number_test gtest_main arty_core)
add_test(NAME number_test COMMAND number_test)
