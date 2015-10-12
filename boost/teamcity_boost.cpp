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

#include <sstream>

#include <boost/test/unit_test_suite_impl.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_log_formatter.hpp>
#include <boost/test/unit_test.hpp>

#include "teamcity_messages.h"

namespace jetbrains { namespace teamcity {

// Custom formatter for TeamCity messages
class TeamcityBoostLogFormatter: public boost::unit_test::unit_test_log_formatter {
    TeamcityMessages messages;
    std::string currentDetails;
    std::string flowId;

public:
    TeamcityBoostLogFormatter(const std::string &_flowId);
    TeamcityBoostLogFormatter();

    virtual ~TeamcityBoostLogFormatter() {}

    virtual void log_start(std::ostream&, boost::unit_test::counter_t test_cases_amount);
    virtual void log_finish(std::ostream&);
    virtual void log_build_info(std::ostream&);

    virtual void test_unit_start(std::ostream&, boost::unit_test::test_unit const& tu);
    virtual void test_unit_finish(std::ostream&,
        boost::unit_test::test_unit const& tu,
        unsigned long elapsed);
    virtual void test_unit_skipped(std::ostream&, boost::unit_test::test_unit const& tu);

    virtual void log_exception(std::ostream&,
        boost::unit_test::log_checkpoint_data const&,
        boost::unit_test::const_string explanation);

    virtual void log_entry_start(std::ostream & out,
        boost::unit_test::log_entry_data const & entry_data,
        log_entry_types let);
    virtual void log_entry_value(std::ostream&, boost::unit_test::const_string value);
    virtual void log_entry_finish(std::ostream&);
};

// Fake fixture to register formatter
struct TeamcityFormatterRegistrar {
    TeamcityFormatterRegistrar() {
        if (underTeamcity()) {
            boost::unit_test::unit_test_log.set_formatter(new TeamcityBoostLogFormatter());
            boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_test_units);
        }
    }
};

BOOST_GLOBAL_FIXTURE(TeamcityFormatterRegistrar)

// Formatter implementation
static std::string toString(boost::unit_test::const_string bstr) {
    std::stringstream ss;

    ss << bstr;

    return ss.str();
}

TeamcityBoostLogFormatter::TeamcityBoostLogFormatter(const std::string &_flowId)
: flowId(_flowId)
{}

TeamcityBoostLogFormatter::TeamcityBoostLogFormatter()
: flowId(getFlowIdFromEnvironment())
{}

void TeamcityBoostLogFormatter::log_start(std::ostream &/*out*/, boost::unit_test::counter_t /*test_cases_amount*/)
{}

void TeamcityBoostLogFormatter::log_finish(std::ostream &/*out*/)
{}

void TeamcityBoostLogFormatter::log_build_info(std::ostream &/*out*/)
{}

void TeamcityBoostLogFormatter::test_unit_start(std::ostream &out, boost::unit_test::test_unit const& tu) {
    messages.setOutput(out);

    if (tu.p_type == boost::unit_test::tut_case) {
        messages.testStarted(tu.p_name, flowId);
    } else {
        messages.suiteStarted(tu.p_name, flowId);
    }

    currentDetails.clear();
}

void TeamcityBoostLogFormatter::test_unit_finish(std::ostream &out, boost::unit_test::test_unit const& tu, unsigned long elapsed) {
    messages.setOutput(out);

    boost::unit_test::test_results const& tr = boost::unit_test::results_collector.results(tu.p_id);
    if (tu.p_type == boost::unit_test::tut_case) {
        if(!tr.passed()) {
            if(tr.p_skipped) {
                messages.testIgnored(tu.p_name, "ignored", flowId);
            } else if (tr.p_aborted) {
                messages.testFailed(tu.p_name, "aborted", currentDetails, flowId);
            } else {
                messages.testFailed(tu.p_name, "failed", currentDetails, flowId);
            }
        }

        messages.testFinished(tu.p_name, elapsed / 1000, flowId);
    } else {
        messages.suiteFinished(tu.p_name, flowId);
    }
}

void TeamcityBoostLogFormatter::test_unit_skipped(std::ostream &/*out*/, boost::unit_test::test_unit const& /*tu*/)
{}

void TeamcityBoostLogFormatter::log_exception(std::ostream &out, boost::unit_test::log_checkpoint_data const &, boost::unit_test::const_string explanation) {
    std::string what = toString(explanation);

    out << what << std::endl;
    currentDetails += what + "\n";
}

void TeamcityBoostLogFormatter::log_entry_start(std::ostream & out, boost::unit_test::log_entry_data const & entry_data, log_entry_types /*let*/)
{
    std::stringstream ss(std::ios_base::out);

    out << entry_data.m_file_name << "(" << entry_data.m_line_num << "): ";
    ss  << entry_data.m_file_name << "(" << entry_data.m_line_num << "): ";

    currentDetails += ss.str();
}

void TeamcityBoostLogFormatter::log_entry_value(std::ostream &out, boost::unit_test::const_string value) {
    out << value;
    currentDetails += toString(value);
}

void TeamcityBoostLogFormatter::log_entry_finish(std::ostream &out) {
    out << std::endl;
    currentDetails += "\n";
}

}}                                                          // namespace teamcity, jetbrains
