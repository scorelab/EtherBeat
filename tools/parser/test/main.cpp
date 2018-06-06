/*
 * Created by prabushitha on 6/6/18.
*/
#include <gtest/gtest.h>

TEST(Testtest, OneEqOneTest){
    EXPECT_EQ(1,1);
}

int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
