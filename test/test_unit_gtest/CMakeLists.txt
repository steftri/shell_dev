set(TEST_NAME test_unit_gtest)


# CMake configuration for unit tests
add_executable(${TEST_NAME} test_unit_gtest.cpp)

# Link the library to the integration tests
target_link_libraries(${TEST_NAME} PRIVATE TinyShell)

# Enable testing for this target
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
