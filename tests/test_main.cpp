//
// Created by bob on 17.11.20.
//

// Google Test framework
#include <gtest/gtest.h>

#include <iostream>

#include "version/version.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(TestUtils, valgrind_check) {
    // Running this test is only useful with valgrind memory check tool enabled.
    // This test mishandles memory on purpose to make sure if the error is detected by valgrind.
    int x, y=0;
    if (x == 42) y = 2; // Should give UninitCondition warning!
    else y++;
    
    FAIL();
}

TEST(TestUtils, program_version) {
    std::cout << "Program version: " << TestProj::Version::version() << std::endl;
    std::cout << "Git SHA1 hash: " << TestProj::Version::git_sha1() << std::endl;
    std::cout << "Integer versions numbers. "
                 "Major version: " << TestProj::Version::version_major()
        << ". Minor version: " << TestProj::Version::version_minor() <<
        ". Release version: " << TestProj::Version::version_release() << std::endl;
}
