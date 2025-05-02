# CMake configuration for unit tests
add_executable(UnitTests test_unit_gtest.cpp)

# Link the library to the unit tests
target_link_libraries(UnitTests PRIVATE TinyShell)

# Enable testing for this target
add_test(NAME UnitTests COMMAND UnitTests)
