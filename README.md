teamcity-cpp
============

[![official project](http://jb.gg/badges/official.svg)](https://confluence.jetbrains.com/display/ALL/JetBrains+on+GitHub)
[![GitHub license](https://img.shields.io/badge/license-Apache%20License%202.0-blue.svg?style=flat)](http://www.apache.org/licenses/LICENSE-2.0)
[![Travis Build Status](https://travis-ci.org/JetBrains/teamcity-cpp.svg?branch=master)](https://travis-ci.org/JetBrains/teamcity-cpp)

C++ Tests Reporting to TeamCity Continuous Integration Server

Repository consist of C++ files to include in your project that automatically report unit tests to the TeamCity server if run within TeamCity.

The reporting is performed via TeamCity service messages

Alternatively, you can use the [XML Report Processing](https://confluence.jetbrains.com/display/TCD9/XML+Report+Processing) build feature to import results from running tests to TeamCity.
You can also use it for frameworks not yet supported, such as CxxTest

See http://confluence.jetbrains.com/display/TW/Cpp+Unit+Test+Reporting for downloads and more information
