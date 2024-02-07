

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