

#ifndef H_TEAMCITY_MESSAGES
#define H_TEAMCITY_MESSAGES

#include <iostream>
#include <string>

namespace jetbrains {
namespace teamcity {

std::string getFlowIdFromEnvironment();
bool underTeamcity();

class TeamcityMessages {
    std::ostream *m_out;

protected:
    std::string escape(const std::string &s);

    void openMsg(const std::string &name);
    void writeProperty(const std::string &name, const std::string &value);
    void closeMsg();

public:
    static const bool StdErr = true;
    static const bool StdOut = false;

    TeamcityMessages();

    void setOutput(std::ostream &);

    void suiteStarted(const std::string &name, const std::string &flowid =  std::string());
    void suiteFinished(const std::string &name, const std::string &flowid =  std::string());

    void testStarted(const std::string &name, const std::string &flowid =  std::string(), bool captureStandardOutput = false);
    void testFailed(const std::string &name, const std::string &message, const std::string &details, const std::string &flowid =  std::string());
    void testIgnored(const std::string &name, const std::string &message, const std::string &flowid =  std::string());
    void testOutput(const std::string &name, const std::string &output, const std::string &flowid, bool isStdErr = StdOut);
    void testFinished(const std::string &name, int durationMs = -1, const std::string &flowid = std::string());
};

}
}

#endif /* H_TEAMCITY_MESSAGES */