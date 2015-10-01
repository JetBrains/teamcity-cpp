/* Copyright 2011 JetBrains s.r.o.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * $Revision: 88625 $
*/

#include <iostream>
#include <stdexcept>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "teamcity_cppunit.h"

using namespace CPPUNIT_NS;
using namespace std;

class MyTest : public TestCase {
    CPPUNIT_TEST_SUITE(MyTest);
    CPPUNIT_TEST(testHelloWorld);
    CPPUNIT_TEST(testException);
    CPPUNIT_TEST(testCerr);
    CPPUNIT_TEST(testAssert);
    CPPUNIT_TEST(testAssertThrow);
    CPPUNIT_TEST(testAssertEqual);
    CPPUNIT_TEST(testNothing);
    CPPUNIT_TEST_SUITE_END();

public:
    void testHelloWorld() {
        cout << "Hello, cout" << endl;
    }

    void testCerr() {
        cerr << "Hello, cerr!" << endl;
    }

    void testException() {
        throwRuntimeException();
    }
    
    void testAssertEqual() {
        CPPUNIT_ASSERT_EQUAL(2, 1);
    }

    void testNothing() {}
    
    void testAssertThrow() {
        CPPUNIT_ASSERT_THROW(throwRuntimeException(), logic_error);
    }
    
    void testAssert() {
        CPPUNIT_ASSERT(false);
    }
    
    void throwRuntimeException() {
        throw runtime_error("runtime exception text");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MyTest);

static void run() {
    CPPUNIT_NS::TestResult controller;

    jetbrains::teamcity::TeamcityProgressListener tc;
    controller.addListener(&tc);

    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);
}

int main(int /*argc*/, char ** /*argv*/) {
    run();
    return 0;
}
