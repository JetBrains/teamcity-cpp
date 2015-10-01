#include <gtest/gtest.h>
#include "teamcity_gtest.h"

TEST(TestCase1, SuccessfulTest1) {
    EXPECT_EQ(1, 1);
}
TEST(TestCase1, MultipleFailures) {
    EXPECT_EQ(1, 2);
    EXPECT_EQ(2, 3);
    EXPECT_EQ(3, 4);
}
TEST(TestCase2, SuccessfulTest2) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if (jetbrains::teamcity::underTeamcity()) {
        ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
        // Add unique flowId parameter if you want to run test processes in parallel
        // See http://confluence.jetbrains.net/display/TCD6/Build+Script+Interaction+with+TeamCity#BuildScriptInteractionwithTeamCity-MessageFlowId
        listeners.Append(new jetbrains::teamcity::TeamcityGoogleTestEventListener());
    }

    return RUN_ALL_TESTS();
}
