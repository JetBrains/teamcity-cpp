CppUnit listener for TeamCity
-----------------------------

To report your test results to TeamCity server
include teamcity_messages.* and teamcity_cppunit.*
to your project and modify "main" function
as shown in example.cpp
(around jetbrains::teamcity::underTeamcity and jetbrains::teamcity::TeamcityProgressListener)

Technical details
-----------------

Reporting is implemented by writing TeamCity service messages to stdout.

See
http://www.jetbrains.net/confluence/display/TCD3/Build+Script+Interaction+with+TeamCity
for more details.

Contact information
-------------------

Mail to teamcity-feedback@jetbrains.com or see other options at

http://www.jetbrains.com/support/teamcity

License
-------

Apache, version 2.0
http://www.apache.org/licenses/LICENSE-2.0
