cmake_minimum_required(VERSION 3.16)
message("CMake version: ${CMAKE_VERSION}")
project(test-project
        VERSION 0.0.1
        LANGUAGES C CXX
        DESCRIPTION "Test project"
        )

# Allows `include`'ing custom CMake modules from this directory.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# ----------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------- CMAKE OPTIONS --------------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

# !!!!!
# Note:
# CMake saves options in the cache (`CMakeCache.txt`).
# If CMake `option` default values are changed in this `CMakeLists.txt` file, the old values from the cache will
# continue to be used. Delete the cache for such changes to take effect.
# !!!!!

option(BUILD_NOTESTS
        "Try to build using as few dependencies as possible. Does not build tests."

        OFF
        )


option(TIMESTAMPS_PROFILING_ENABLED
        "Enables the output of timestamps (can be used for out-of-cpu profiling and debug messages).
        The effect is that the PRINT_WITH_TIMESTAMP macro (default empty) is set to output messages with timestamps
        , to stderr, caused by a compile definition (same name `TIMESTAMPS_PROFILING_ENABLED`).
        "

        ON
        )

if (TIMESTAMPS_PROFILING_ENABLED)
    message(STATUS "#define compile definition TIMESTAMPS_PROFILING_ENABLED.")
    add_compile_definitions(TIMESTAMPS_PROFILING_ENABLED)
endif ()


# Needed for code coverage with GCC compiler:
#add_compile_options(--coverage)
#add_link_options(--coverage)
#add_compile_options(-g3) # more debug information. Could also use -ggdb3 for gdb debugger.
# Enable GCC compiler warnings:
#add_compile_options(-Wall -Wextra -Wpedantic -Wunused -Woverloaded-virtual
#        -Wduplicated-cond -Wlogical-op -Wduplicated-branches -Wuseless-cast -Wduplicated-cond -Wnull-dereference
#        -Wnon-virtual-dtor -Wcast-align -Wformat=2
#        -Wold-style-cast  -Wshadow -Wconversion -Wsign-conversion -Wdouble-promotion
#        )
# optimization should be handled by build type,
# cmake -DCMAKE_BUILD_TYPE=Release flag uses (on my system) -O3 optimization (which may not be always desired)?!
# Pass VERBOSE=1 to make to see all the options used.
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
# ----------------------------------------------------------------------------------------------------------------------
# ------------------------------------ GET VERSION FROM GIT AND INCLUDE IN BINARIES ------------------------------------
# ----------------------------------------------------------------------------------------------------------------------
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git" AND NOT AFF3CT_OVERRIDE_VERSION)
    message(FATAL_ERROR "The '.git' folder can't be found. This program cannot be compiled unless it is cloned "
            "from a Git repository. Please do not download archives and make a Git clone instead.")
else ()
    include(GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_SHA1)
    git_describe(GIT_VERSION "--tags" "--dirty")

    # Extract each part of the information from `git describe` output (called by `git_describe` command in module).
    string(REGEX REPLACE "^v" "" TESTPROJ_VERSION_FULL ${GIT_VERSION})
    string(REGEX REPLACE "-.*" "" TESTPROJ_VERSION ${TESTPROJ_VERSION_FULL})
    string(REPLACE "-" "" TESTPROJ_BUILD ${TESTPROJ_VERSION_FULL})
    string(REPLACE "${TESTPROJ_VERSION}" "" TESTPROJ_BUILD ${TESTPROJ_BUILD})
    # TODO deal with crash when no git tag is available...
    #    string(REGEX REPLACE "(.*)\\..*\\..*" "\\1" TESTPROJ_VERSION_MAJOR ${TESTPROJ_VERSION})
    #    string(REGEX REPLACE ".*\\.(.*)\\..*" "\\1" TESTPROJ_VERSION_MINOR ${TESTPROJ_VERSION})
    #    string(REGEX REPLACE ".*\\..*\\.(.*)" "\\1" TESTPROJ_VERSION_PATCH ${TESTPROJ_VERSION})

    message("Obtained git repository info: ${GIT_REFSPEC}, ${GIT_SHA1}, ${GIT_VERSION}")
    #    message("${TESTPROJ_VERSION_FULL}
    #     ${TESTPROJ_VERSION}
    #      ${TESTPROJ_BUILD}
    #       ${TESTPROJ_VERSION_MAJOR}
    #        ${TESTPROJ_VERSION_MINOR}
    #         ${TESTPROJ_VERSION_PATCH}
    #            ")

    # Write version information into source file to be compiled into the binary.
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version/version.cpp.in"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/version/version.cpp" @ONLY)

    # TODO generate cmake config version file to link with library
endif ()

# ----------------------------------------------------------------------------------------------------------------------
# ---------------------------------------------------- WEIRD STUFF -----------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------
# Required to enable testing for link-time optimization.
# https://cmake.org/cmake/help/v3.11/module/CheckIPOSupported.html
include(CheckIPOSupported)


# Use link-time optimization if allowed. # TODO check if this works and if/why it is not automatic!
check_ipo_supported(RESULT ipo_supported)
if (ipo_supported)
    set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "Link-time optimization (IPO) is enabled.")
else ()
    message(WARNING "Link-time optimization is not supported.")
endif ()


# We attempt to use ccache to speed up the build. # TODO get this to work and find out what it does exactly
find_program(CCACHE_FOUND "ccache")
if (CCACHE_FOUND)
    message(STATUS "Using ccache for building.")
    set_property(GLOBAL PROPERTY LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message(STATUS "ccache is being used to speed up the build.")
else ()
    message(WARNING "Using ccache to speed up the build is not possible.")
endif (CCACHE_FOUND)


# If no installation prefix is given manually, install locally.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING
            "The install location"
            FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# ----------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------- INCLUDE SUBDIRECTORIES -------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

#add_subdirectory(libs)

add_subdirectory(src)

#add_subdirectory(examples)

if(BUILD_NOTESTS)
# Enable testing. This is not necessary for using GTest,
# only provides integration with CTest (CMake test suite)
# Note that this line has to be located before adding the sub-directory with the tests.
# Having this line in the test/CMakeLists.txt file doesn't work (I don't know why).
enable_testing()

add_subdirectory(tests) # If you comment out this line, the project can build without GTest library installed.
else()
    message(INFO "Not building unit tests as per user request.")
endif(BUILD_NOTESTS)
