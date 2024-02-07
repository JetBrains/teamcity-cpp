

#include <sstream>

#include "cppunit/Test.h"
#include "cppunit/Exception.h"

#include "teamcity_cppunit.h"

using namespace CPPUNIT_NS;
using namespace std;

namespace jetbrains {
namespace teamcity {

TeamcityProgressListener::TeamcityProgressListener()
{
    flowid = getFlowIdFromEnvironment();
}

TeamcityProgressListener::TeamcityProgressListener(const std::string& _flowid)
{
    flowid = _flowid;
}

TeamcityProgressListener::~TeamcityProgressListener()
{}

void TeamcityProgressListener::startTest(Test *test) {
    messages.testStarted(test->getName(), flowid);
}

static string sourceLine2string(const SourceLine &sline) {
    stringstream ss;
        
    ss << sline.fileName() << ":" << sline.lineNumber();
    
    return ss.str();
}

void TeamcityProgressListener::addFailure(const TestFailure &failure) {
    const Exception *e = failure.thrownException();
    
    string details = e->message().details();
    
    if (e->sourceLine().isValid()) {
        details.append(" at ");
        details.append(sourceLine2string(e->sourceLine()));
        details.append("\n");
    }
    
    messages.testFailed(
        failure.failedTest()->getName(),
        e->message().shortDescription(),
        details,
        flowid
    );
}

void TeamcityProgressListener::endTest(Test *test) {
    messages.testFinished(test->getName(), -1, flowid);
}

void TeamcityProgressListener::startSuite(Test *test) {
    messages.suiteStarted(test->getName(), flowid);
}

void TeamcityProgressListener::endSuite(Test *test) {
    messages.suiteFinished(test->getName(), flowid);
}

}
}