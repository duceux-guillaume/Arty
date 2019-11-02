add_executable(math_test math_test.cpp)
target_link_libraries(math_test gtest_main arty)
add_test(NAME math_test COMMAND math_test)

add_executable(blackboard_test blackboard_test.cpp)
target_link_libraries(blackboard_test gtest_main arty)
add_test(NAME blackboard_test COMMAND blackboard_test)

add_executable(collision_test collision_test.cpp)
target_link_libraries(collision_test gtest_main arty)
add_test(NAME collision_test COMMAND collision_test)
add_custom_target(TARGET collision_test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy
   "${CMAKE_SOURCE_DIR}/test/assets/test_cube.obj"
   "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
)

add_executable(geometry_test geometry_test.cpp)
target_link_libraries(geometry_test gtest_main arty)
add_test(NAME geometry_test COMMAND geometry_test)
