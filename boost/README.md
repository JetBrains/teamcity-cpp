Boost listener for TeamCity
---------------------------

To report your test results to TeamCity server include teamcity_messages.* and teamcity_boost.* to your project.

teamcity_boost.cpp registers global fixture
( http://www.boost.org/doc/libs/1_38_0/libs/test/doc/html/utf/user-guide/fixture/global.html )
to replace output formatter if run under TeamCity. If you prefer to place teamcity_boost.cpp in a static library, then you'll need to enable teamcity fixture by calling `jetbrains::teamcity::TeamcityGlobalFixture()` from your code.

If you have tests with test parameters, see PARAM_TEST_CASES.txt for quick solution.

Technical details
-----------------

Reporting is implemented by writing TeamCity service messages to stdout.

See http://www.jetbrains.net/confluence/display/TCD3/Build+Script+Interaction+with+TeamCity for more details.

Contact information
-------------------

http://www.jetbrains.com/support/teamcity

License
-------

Apache, version 2.0
http://www.apache.org/licenses/LICENSE-2.0
