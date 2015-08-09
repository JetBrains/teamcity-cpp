TEAMCITY_PROJECT_NAME = test
export TEAMCITY_PROJECT_NAME

CXX ?= c++

COMMON_FILES = common/teamcity_messages.cpp common/teamcity_messages.h
BOOST_FILES = $(COMMON_FILES) \
	boost/teamcity_boost.cpp \
	boost/boost_test.cpp
CPPUNIT_FILES = $(COMMON_FILES) \
	cppunit/teamcity_cppunit.cpp cppunit/teamcity_cppunit.h \
	cppunit/cppunit_test.cpp
GTEST_FILES = $(COMMON_FILES) \
	gtest/teamcity_gtest.cpp gtest/teamcity_gtest.h \
	gtest/gtest_test.cpp

.PHONY: all test dist

all: test

# Collection of warnings from https://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings
CXXFLAGS := -Icommon -g -O0 \
	-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
	-Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast \
	-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
	-Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -Wno-unknown-warning-option

boost_test: $(BOOST_FILES) Makefile
	$(CXX) $(CXXFLAGS) -DBOOST_TEST_DYN_LINK -o $@ $(filter %.cpp, $(BOOST_FILES)) -lboost_unit_test_framework

cppunit_test: $(CPPUNIT_FILES) Makefile
	$(CXX) $(CXXFLAGS) -o \
		$@ $(filter %.cpp, $(CPPUNIT_FILES)) -lcppunit

gtest_test: $(GTEST_FILES) Makefile
	$(CXX) $(CXXFLAGS) -o \
		$@ $(filter %.cpp, $(GTEST_FILES)) -lgtest -lpthread


BOOST_OUTPUT = boost/boost_test.output
CPPUNIT_OUTPUT = cppunit/cppunit_test.output
GTEST_OUTPUT = gtest/gtest_test.output

test: boost_test cppunit_test gtest_test
	./boost_test >$(BOOST_OUTPUT).tmp 2>&1 ||:
	diff -Nru $(BOOST_OUTPUT).gold $(BOOST_OUTPUT).tmp && rm -f $(BOOST_OUTPUT).tmp
	
	TEAMCITY_PROCESS_FLOW_ID=myFlowId ./boost_test >$(BOOST_OUTPUT).flowId.tmp 2>&1 ||:
	diff -Nru $(BOOST_OUTPUT).flowId.gold $(BOOST_OUTPUT).flowId.tmp && rm -f $(BOOST_OUTPUT).flowId.tmp
	
	./cppunit_test >$(CPPUNIT_OUTPUT).tmp 2>&1 ||:
	diff -Nru $(CPPUNIT_OUTPUT).gold $(CPPUNIT_OUTPUT).tmp && rm -f $(CPPUNIT_OUTPUT).tmp
	
	TEAMCITY_PROCESS_FLOW_ID=myFlowId ./cppunit_test >$(CPPUNIT_OUTPUT).flowId.tmp 2>&1 ||:
	diff -Nru $(CPPUNIT_OUTPUT).flowId.gold $(CPPUNIT_OUTPUT).flowId.tmp && rm -f $(CPPUNIT_OUTPUT).flowId.tmp

	./gtest_test >$(GTEST_OUTPUT).tmp 2>&1 ||:
	diff -Nru $(GTEST_OUTPUT).gold $(GTEST_OUTPUT).tmp && rm -f $(GTEST_OUTPUT).tmp

	TEAMCITY_PROCESS_FLOW_ID=myFlowId ./gtest_test >$(GTEST_OUTPUT).flowId.tmp 2>&1 ||:
	diff -Nru $(GTEST_OUTPUT).flowId.gold $(GTEST_OUTPUT).flowId.tmp && rm -f $(GTEST_OUTPUT).flowId.tmp

	@echo "<<< Tests OK >>>"

VERSION = $(shell cat VERSION)

clean:
	rm -f boost_test cppunit_test gtest_test

dist:
	rm -f teamcity-*-$(VERSION).zip
	zip -rj9 teamcity-cppunit-$(VERSION).zip \
		cppunit/teamcity_cppunit.* common/teamcity_messages.* \
	    cppunit/example.cpp cppunit/README.txt
	zip -rj9 teamcity-boost-$(VERSION).zip \
		boost/teamcity_boost.* common/teamcity_messages.* boost/README.txt
	zip -rj9 teamcity-gtest-$(VERSION).zip \
		gtest/teamcity_gtest.* common/teamcity_messages.* \
	    gtest/example.cpp gtest/README.txt
