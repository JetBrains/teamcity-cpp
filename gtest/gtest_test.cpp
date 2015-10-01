/* Copyright 2015 Paul Shmakov
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
 */
#include <stdexcept>

#include <gtest/gtest.h>
#include "teamcity_gtest.h"

void throwRuntimeException();

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
void throwRuntimeException() {
    throw std::runtime_error("runtime exception text");
}
TEST(TestCase2, FailedThrow) {
    ASSERT_THROW(throwRuntimeException(), std::logic_error);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new jetbrains::teamcity::TeamcityGoogleTestEventListener());

    return RUN_ALL_TESTS();
}
