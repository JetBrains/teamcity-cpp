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
