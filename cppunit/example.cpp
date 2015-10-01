#include <iostream>
#include <stdexcept>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "teamcity_cppunit.h"

using namespace CppUnit;
using namespace std;

class MyTest : public TestCase {
    CPPUNIT_TEST_SUITE(MyTest);
    CPPUNIT_TEST(testHelloWorld);
    CPPUNIT_TEST(testAssertEqual);
    CPPUNIT_TEST(testNothing);
    CPPUNIT_TEST_SUITE_END();

public:
    void testHelloWorld() {
        cout << "Hello, world" << endl;
    }

    void testAssertEqual() {
        CPPUNIT_ASSERT_EQUAL(2, 1);
    }

    void testNothing() {}
};

CPPUNIT_TEST_SUITE_REGISTRATION(MyTest);

int main(int argc, char **argv) {
    // Create the event manager and test controller
    TestResult controller;
    
    // Add a listener that collects test result
    TestResultCollector result;
    controller.addListener(&result);
    
    // Add the top suite to the test runner
    TestRunner runner;
    runner.addTest(TestFactoryRegistry::getRegistry().makeTest());

    // Listen to progress
    TestListener *listener;
    
    if (jetbrains::teamcity::underTeamcity()) {
        // Add unique flowId parameter if you want to run test processes in parallel
        // See http://confluence.jetbrains.net/display/TCD6/Build+Script+Interaction+with+TeamCity#BuildScriptInteractionwithTeamCity-MessageFlowId
        listener = new jetbrains::teamcity::TeamcityProgressListener();
    } else {
        listener = new BriefTestProgressListener();
    }
    controller.addListener(listener);

    // Run tests
    runner.run(controller);
    
    delete listener;
    
    return result.wasSuccessful() ? 0 : 1;
}
