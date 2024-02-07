

#ifndef H_TEAMCITY_CPPUNIT
#define H_TEAMCITY_CPPUNIT

#include <cppunit/TestFailure.h>
#include <cppunit/TestListener.h>
#include <string>

#include "teamcity_messages.h"

namespace jetbrains {
namespace teamcity {

class TeamcityProgressListener: public CPPUNIT_NS::TestListener {
    TeamcityMessages messages;
    
public:
    TeamcityProgressListener(const std::string& _flowid);
    TeamcityProgressListener();
    ~TeamcityProgressListener();

    void startTest(CPPUNIT_NS::Test *test);
    void addFailure(const CPPUNIT_NS::TestFailure &failure);
    void endTest(CPPUNIT_NS::Test *test);
    void startSuite(CPPUNIT_NS::Test *test);
    void endSuite(CPPUNIT_NS::Test *test);
    
private:
    std::string flowid;

    // Prevents the use of the copy constructor.
    TeamcityProgressListener(const TeamcityProgressListener &copy);

    // Prevents the use of the copy operator.
    void operator =(const TeamcityProgressListener &copy);
};

}
}

#endif /* H_TEAMCITY_CPPUNIT */