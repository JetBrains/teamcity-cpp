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

#include "teamcity_messages.h"

#include <cstdlib>
#include <sstream>

namespace jetbrains {
namespace teamcity {

std::string getFlowIdFromEnvironment() {
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && !defined(__CYGWIN__) && !defined(__MINGW32__)
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
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && !defined(__CYGWIN__) && !defined(__MINGW32__)
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
: m_out(&std::cout)
{}

void TeamcityMessages::setOutput(std::ostream &out) {
    m_out = &out;
}

std::string TeamcityMessages::escape(const std::string &s) {
    std::string result;
    result.reserve(s.length());

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

void TeamcityMessages::openMsg(std::stringstream &str, const std::string &name) {
    // endl for http://jetbrains.net/tracker/issue/TW-4412
    str << std::endl << "##teamcity[" << name;
}

void TeamcityMessages::closeMsg(std::stringstream &str) {
    str << "]";
    // endl for http://jetbrains.net/tracker/issue/TW-4412
    str << std::endl;
}

void TeamcityMessages::writeProperty(std::stringstream &str, const std::string &name, const std::string &value) {
    str << " " << name << "='" << escape(value) << "'";
}

void TeamcityMessages::suiteStarted(const std::string &name, const std::string &flowid) {
    std::stringstream str;
    openMsg(str, "testSuiteStarted");
    writeProperty(str, "name", name);
    if(flowid.length() > 0) {
        writeProperty(str, "flowId", flowid);
    }
    closeMsg(str);

    *m_out << str.str();
}

void TeamcityMessages::suiteFinished(const std::string &name, const std::string &flowid) {
    std::stringstream str;
    openMsg(str, "testSuiteFinished");
    writeProperty(str, "name", name);
    if(flowid.length() > 0) {
        writeProperty(str, "flowId", flowid);
    }
    closeMsg(str);

    *m_out << str.str();
}

void TeamcityMessages::testStarted(const std::string &name, const std::string &flowid, bool captureStandardOutput) {
    std::stringstream str;
    openMsg(str, "testStarted");
    writeProperty(str, "name", name);
    if(flowid.length() > 0) {
        writeProperty(str, "flowId", flowid);
    }

    if(captureStandardOutput) {
        writeProperty(str, "captureStandardOutput", "true"); // false by default
    }

    closeMsg(str);

    *m_out << str.str();
}

void TeamcityMessages::testFinished(const std::string &name, int durationMs, const std::string &flowid) {
    std::stringstream str;
    openMsg(str, "testFinished");

    writeProperty(str, "name", name);

    if(flowid.length() > 0) {
        writeProperty(str, "flowId", flowid);
    }

    if(durationMs >= 0) {
        std::stringstream out(std::ios_base::out);
        out << durationMs;
        writeProperty(str, "duration", out.str());
    }

    closeMsg(str);

    *m_out << str.str();
}

void TeamcityMessages::testFailed(const std::string &name, const std::string &message, const std::string &details, const std::string &flowid) {
    std::stringstream str;
    openMsg(str, "testFailed");
    writeProperty(str, "name", name);
    writeProperty(str, "message", message);
    writeProperty(str, "details", details);
    if(flowid.length() > 0) {
        writeProperty(str, "flowId", flowid);
    }
    closeMsg(str);

    *m_out << str.str();
}

void TeamcityMessages::testIgnored(const std::string &name, const std::string &message, const std::string &flowid) {
    std::stringstream str;
    openMsg(str, "testIgnored");
    writeProperty(str, "name", name);
    writeProperty(str, "message", message);
    if(flowid.length() > 0) {
        writeProperty(str, "flowId", flowid);
    }
    closeMsg(str);

    *m_out << str.str();
}

void TeamcityMessages::testOutput(const std::string &name, const std::string &output, const std::string &flowid, bool isStdError) {
    std::stringstream str;
    openMsg(str, isStdError ? "testStdErr" : "testStdOut");
    writeProperty(str, "name", name);
    writeProperty(str, "out", output);
    if(flowid.length() > 0) {
        writeProperty(str, "flowId", flowid);
    }
    closeMsg(str);

    *m_out << str.str();
}

}
}
