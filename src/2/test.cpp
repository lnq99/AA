#include <gtest/gtest.h>
#include "matrix/matrix.h"


class NormalTest : public testing::Test
{
public:
    Matrix<2,3> A = { 2,4,3,1,-3,2 };
    Matrix<3,2> B = { 2,-3,4,4,2,3 };
    Matrix<2,2> C = { 26,19,-6,-9 };
    Matrix<3,3> D = { 1,17,0,12,4,20,7,-1,12 };
};

class ZeroTest : public testing::Test
{
public:
    Matrix<2,2> A;
    Matrix<2,2> B = { 2,3,4,4 };
    Matrix<2,2> C;
};

class ErrorTest : public testing::Test
{
public:
    Matrix<2,2> A;
    Matrix<1,2> B;
};



TEST_F(ZeroTest, MulStandard)
{
    EXPECT_EQ(A * B, C);
}

TEST_F(NormalTest, MulStandard)
{
    EXPECT_EQ(A * B, C);
    EXPECT_EQ(B * A, D);
}

TEST_F(ErrorTest, MulStandard)
{
    EXPECT_ANY_THROW(A * B);
}

TEST_F(ZeroTest, MulWinograd)
{
    EXPECT_EQ(A ^ B, C);
}

TEST_F(NormalTest, MulWinograd)
{
    EXPECT_EQ(A ^ B, C);
    EXPECT_EQ(B ^ A, D);
}

TEST_F(ErrorTest, MulWinograd)
{
    EXPECT_ANY_THROW(A ^ B);
}

TEST_F(NormalTest, MulWinogradOpt)
{
    EXPECT_EQ(A & B, C);
    EXPECT_EQ(B & A, D);
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
