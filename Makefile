TEAMCITY_PROJECT_NAME = test
export TEAMCITY_PROJECT_NAME

COMMON_FILES = common/teamcity_messages.cpp common/teamcity_messages.h
BOOST_FILES = $(COMMON_FILES) \
	boost/teamcity_boost.cpp \
	boost/boost_test.cpp
CPPUNIT_FILES = $(COMMON_FILES) \
	cppunit/teamcity_cppunit.cpp cppunit/teamcity_cppunit.h \
	cppunit/cppunit_test.cpp

.PHONY: all test dist

all: test

CXXFLAGS := -Icommon -g -O0

boost_test: $(BOOST_FILES)
	g++ $(CXXFLAGS) -DBOOST_TEST_DYN_LINK -o \
		$@ $(filter %.cpp, $(BOOST_FILES)) -lboost_unit_test_framework-mt

cppunit_test: $(CPPUNIT_FILES)
	g++ $(CXXFLAGS) -o \
		$@ $(filter %.cpp, $(CPPUNIT_FILES)) -lcppunit

BOOST_OUTPUT = boost/boost_test.output
CPPUNIT_OUTPUT = cppunit/cppunit_test.output

test: boost_test cppunit_test
	BOOST_TEST_LOG_LEVEL=all ./boost_test >$(BOOST_OUTPUT).tmp 2>&1 ||:
	diff -Nru $(BOOST_OUTPUT).gold $(BOOST_OUTPUT).tmp && rm -f $(BOOST_OUTPUT).tmp
	
	BOOST_TEST_LOG_LEVEL=all TEAMCITY_PROCESS_FLOW_ID=myFlowId ./boost_test >$(BOOST_OUTPUT).flowId.tmp 2>&1 ||:
	diff -Nru $(BOOST_OUTPUT).flowId.gold $(BOOST_OUTPUT).flowId.tmp && rm -f $(BOOST_OUTPUT).flowId.tmp

	./cppunit_test >$(CPPUNIT_OUTPUT).tmp 2>&1 ||:
	diff -Nru $(CPPUNIT_OUTPUT).gold $(CPPUNIT_OUTPUT).tmp && rm -f $(CPPUNIT_OUTPUT).tmp
	
	TEAMCITY_PROCESS_FLOW_ID=myFlowId ./cppunit_test >$(CPPUNIT_OUTPUT).flowId.tmp 2>&1 ||:
	diff -Nru $(CPPUNIT_OUTPUT).flowId.gold $(CPPUNIT_OUTPUT).flowId.tmp && rm -f $(CPPUNIT_OUTPUT).flowId.tmp
	
	@echo "<<< Tests OK >>>"

VERSION = $(shell cat VERSION)

dist:
	rm -f teamcity-*-$(VERSION).zip
	zip -rj9 teamcity-cppunit-$(VERSION).zip \
		cppunit/teamcity_cppunit.* common/teamcity_messages.* \
	    cppunit/example.cpp cppunit/README.txt
	zip -rj9 teamcity-boost-$(VERSION).zip \
		boost/teamcity_boost.* common/teamcity_messages.* boost/README.txt
