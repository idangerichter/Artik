# Unit test
add_executable(unit_tests
        testutils/testutils_sample_primitives.inl
        example_test.cpp samplers/test_simple_sampler.cpp samplers/test_list_sampler.cpp)

target_link_libraries(unit_tests gtest_main gmock_main Lib)
add_test(NAME unit COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)
