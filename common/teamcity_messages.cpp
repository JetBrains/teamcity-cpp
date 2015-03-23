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

#include <stdlib.h>
#include <sstream>

#include "teamcity_messages.h"

using namespace std;

namespace JetBrains {

std::string getFlowIdFromEnvironment() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    char *flowId = NULL;
    size_t sz = 0;
    std::string result;
    if(!_dupenv_s(&flowId, &sz,"TEAMCITY_PROCESS_FLOW_ID")) {
        result = flowId != NULL ? flowId : "";
        free(flowId);
    }
    
    return result;
#else
    const char *flowId = getenv("TEAMCITY_PROCESS_FLOW_ID");
    return flowId == NULL ? "" : flowId;
#endif
}

bool underTeamcity() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    char *teamCityProjectName = 0;
    size_t sz = 0;
    bool result = false;
    if(!_dupenv_s(&teamCityProjectName, &sz, "TEAMCITY_PROJECT_NAME")) {
        result = teamCityProjectName != NULL;
        free(teamCityProjectName);
    }
    
    return result;
#else
    return getenv("TEAMCITY_PROJECT_NAME") != NULL;
#endif
}

TeamcityMessages::TeamcityMessages()
: m_out(&cout)
{}

void TeamcityMessages::setOutput(ostream &out) {
    m_out = &out;
}

string TeamcityMessages::escape(string s) {
    string result;
    
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        
        switch (c) {
        case '\n': result.append("|n"); break;
        case '\r': result.append("|r"); break;
        case '\'': result.append("|'"); break;
        case '|':  result.append("||"); break;
        case ']':  result.append("|]"); break;
        default:   result.append(&c, 1);
        }
    }
    
    return result;
}

void TeamcityMessages::openMsg(const string &name) {
    // endl for http://jetbrains.net/tracker/issue/TW-4412
    *m_out << endl << "##teamcity[" << name;
}

void TeamcityMessages::closeMsg() {
    *m_out << "]";
    // endl for http://jetbrains.net/tracker/issue/TW-4412
    *m_out << endl;
    m_out->flush();
}

void TeamcityMessages::writeProperty(string name, string value) {
    *m_out << " " << name << "='" << escape(value) << "'";
}

void TeamcityMessages::suiteStarted(string name, string flowid) {
    openMsg("testSuiteStarted");
    writeProperty("name", name);
    if(flowid.length() > 0) {
        writeProperty("flowId", flowid);
    }
    
    closeMsg();
}

void TeamcityMessages::suiteFinished(string name, string flowid) {
    openMsg("testSuiteFinished");
    writeProperty("name", name);
    if(flowid.length() > 0) {
        writeProperty("flowId", flowid);
    }
    
    closeMsg();
}

void TeamcityMessages::testStarted(string name, string flowid) {
    openMsg("testStarted");
    writeProperty("name", name);
    if(flowid.length() > 0) {
        writeProperty("flowId", flowid);
    }
    
    closeMsg();
}

void TeamcityMessages::testFinished(string name, int durationMs, string flowid) {
    openMsg("testFinished");

    writeProperty("name", name);

    if(flowid.length() > 0) {
        writeProperty("flowId", flowid);
    }

    if(durationMs >= 0) {
        stringstream out;
        out << durationMs;
        writeProperty("duration", out.str());
    }
    
    closeMsg();
}

void TeamcityMessages::testFailed(string name, string message, string details, string flowid) {
    openMsg("testFailed");
    writeProperty("name", name);
    writeProperty("message", message);
    writeProperty("details", details);
    if(flowid.length() > 0) {
        writeProperty("flowId", flowid);
    }
    
    closeMsg();
}

void TeamcityMessages::testIgnored(std::string name, std::string message, string flowid) {
    openMsg("testIgnored");
    writeProperty("name", name);
    writeProperty("message", message);
    if(flowid.length() > 0) {
        writeProperty("flowId", flowid);
    }
    
    closeMsg();
}

}
