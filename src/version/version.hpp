//
// Created by alice on 29.12.20.
// Adapted from AFF3CT library version module.
//

#ifndef TESTPROJ_VERSION_HPP
#define TESTPROJ_VERSION_HPP

#include <string>

namespace TestProj::Version {
    std::string git_sha1();

    std::string version();

    int version_major();

    int version_minor();

    int version_release();
}


#endif //TESTPROJ_VERSION_HPP
