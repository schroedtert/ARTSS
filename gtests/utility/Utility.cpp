/// \file       Utility.cpp
/// \brief      Tests for src/utility/Utility.cpp
/// \date       October 10, 2020
/// \author     N16h7 Hut3r
/// \copyright  <2015-2020> Forschungszentrum Juelich GmbH. All rights reserved.

#include "gtest/gtest.h"
#include "src/utility/Utility.h"

TEST(Utility, coordinateFromLinearIndexAll1) {
    std::vector<size_t> test({0, 0, 1});
    auto ret = Utility::coordinateFromLinearIndex(1, 1, 1);

    EXPECT_EQ(ret.size(), 3);
    for (size_t i=0; i < ret.size(); ++i)
        EXPECT_EQ(ret[i], test[i]);
}

TEST(Utility, coordinateFromLinearIndexAll10) {
    std::vector<size_t> test({0, 1, 0});
    auto ret = Utility::coordinateFromLinearIndex(10, 10, 10);

    EXPECT_EQ(ret.size(), 3);
    for (size_t i=0; i < ret.size(); ++i)
        EXPECT_EQ(ret[i], test[i]);
}

// TEST(Utility, coordinateFromLinearIndexZeros) {
//     ASSERT_DEATH(Utility::coordinateFromLinearIndex(0, 0, 0), "");
// }
