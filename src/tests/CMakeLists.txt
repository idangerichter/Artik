# Unit test
file(GLOB_RECURSE unit_tests_sources unit_tests/*.* unit_tests/*/.*.)
add_executable(unit_tests testutils/testutils_sample_primitives.inl ${unit_tests_sources})
target_link_libraries(unit_tests gtest_main gmock_main Lib)
add_test(NAME unit COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)

# System test
file(GLOB_RECURSE system_tests_sources system_tests/*.* system_tests/*/.*.)
add_executable(system_tests ${system_tests_sources})
target_link_libraries(system_tests Lib)
