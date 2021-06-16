#
# CMakeLists.txt file for building unit tests.
# Adapted from https://github.com/pabloariasal/modern-cmake-sample/blob/master/libjsonutils/test/CMakeLists.txt
# see /opt/local/share/cmake-3.16/Modules/FindGTest.cmake
#

cmake_minimum_required(VERSION 3.17)

# Enable testing. This is not necessary for using GTest,
# only provides integration with CTest (CMake test suite)
enable_testing()

# locate the Google C++ Testing Framework (only needed to run the tests)
# copied from https://cmake.org/cmake/help/v3.0/module/FindGTest.html
#find_package(GTest QUIET)
find_package(GTest REQUIRED)

# Allows using the `gtest_discover_tests` CMake command.
include(GoogleTest)

# TODO THIS DOESN"T WORK YET AND WILL FAIL!!!! TODO remove completely?!
# NOTE: the upper case GTEST! CK
#if(NOT GTEST_FOUND)
#    # Download and unpack googletest at configure time
#    # but only if needed! CK
#    configure_file(${CMAKE_SOURCE_DIR}/cmake/GoogleTest-CMakeLists.txt.in
#            ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
#    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#            RESULT_VARIABLE result
#            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
#    if(result)
#        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
#    endif()
#
#    execute_process(COMMAND ${CMAKE_COMMAND} --build .
#            RESULT_VARIABLE result
#            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
#    if(result)
#        message(FATAL_ERROR "Build step for googletest failed: ${result}")
#    endif()
#
#    # Add googletest directly to our build. This defines
#    # the gtest and gtest_main targets.
#    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
#            ${CMAKE_BINARY_DIR}/googletest-build
#            EXCLUDE_FROM_ALL)
#endif(NOT GTEST_FOUND)


# building unit tests. Uses Google Tests framework, which has to be installed on the target system.
# NOTE: For the file-IO tests to work, `tests` must be the cwd of the test execution. If this fails, either adapt the
# path variable tests_folder_path in test_main.cpp or change the cwd of the executable.
add_executable(unit_tests_test_proj test_main.cpp
        # -------- Actual Unit tests --------

        )


target_compile_features(unit_tests_test_proj
        PUBLIC cxx_std_17)


target_include_directories(unit_tests_test_proj
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )


target_link_libraries(unit_tests_test_proj
        PRIVATE
        # Needed for testing:
        #        ${GTEST_LIBRARIES} # macro provided by find_package(GTest REQUIRED) earlier
        #        ${GTEST_MAIN_LIBRARIES} # macro provided by find_package(GTest REQUIRED) earlier
        GTest::Main  # Some people use GTest:GTest instead. TODO  Is there a differece?
        pthread # TODO is this actually needed for testing? Also some people use Threads::Threads or something like that

        # To be tested:
        TestLib::TestLib
        )

# TODO add tests via CTest (see what happens with CLion)
gtest_discover_tests(unit_tests_test_proj)
