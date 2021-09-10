#
# CMakeLists.txt file for building unit tests.
# Adapted from https://github.com/pabloariasal/modern-cmake-sample/blob/master/libjsonutils/test/CMakeLists.txt
# see /opt/local/share/cmake-3.16/Modules/FindGTest.cmake
#
cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)


# building unit tests. Uses Google Tests framework, which has to be installed on the target system.
# NOTE: For the file-IO tests to work, `tests` must be the cwd of the test execution. If this fails, either adapt the
# path variable tests_folder_path in test_main.cpp or change the cwd of the executable.
add_executable(unit_tests_test_proj test_main.cpp
        # -------- Actual Unit tests --------

        )

target_compile_features(unit_tests_test_proj
        PUBLIC cxx_std_17)

if(ENABLE_COVERAGE)
        target_compile_options(unit_tests_test_proj PUBLIC --coverage -O0 -g)
        target_link_options(unit_tests_test_proj PUBLIC --coverage)
endif()


target_include_directories(unit_tests_test_proj
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )


target_link_libraries(unit_tests_test_proj
        PRIVATE
        # Needed for testing:
        #        ${GTEST_LIBRARIES} # macro provided by find_package(GTest REQUIRED) earlier
        #        ${GTEST_MAIN_LIBRARIES} # macro provided by find_package(GTest REQUIRED) earlier
        gtest_main  # Some people use GTest:GTest instead. TODO  Is there a differece?
        pthread # TODO is this actually needed for testing? Also some people use Threads::Threads or something like that

        # To be tested:
        TestLib::TestLib
        )


include(GoogleTest)
gtest_discover_tests(unit_tests_test_proj)


