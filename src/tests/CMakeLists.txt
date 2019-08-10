# Unit test
add_executable(unit_tests
        testutils/testutils_sample_primitives.inl
        samplers/test_simple_sampler.cpp samplers/test_list_sampler.cpp samplers/test_average_sampler.cpp utils/test_memory_wrapper.cpp)

target_link_libraries(unit_tests gtest_main gmock_main Lib)
add_test(NAME unit COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)
