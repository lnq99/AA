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



TEST_F(ZeroTest, MulWinograd)
{
    EXPECT_EQ(mul_winograd(A, B), C);
}

TEST_F(NormalTest, MulWinograd)
{
    EXPECT_EQ(mul_winograd(A, B), C);
    EXPECT_EQ(mul_winograd(B, A), D);
}


TEST_F(ZeroTest, MulWinogradMultithreading)
{
    EXPECT_EQ(mul_winograd_multithread(A, B), C);
    EXPECT_EQ(mul_winograd_multithread(A, B, 2), C);
    EXPECT_EQ(mul_winograd_multithread(A, B, 4), C);
}

TEST_F(NormalTest, MulWinogradMultithreading)
{
    EXPECT_EQ(mul_winograd_multithread(A, B), C);
    EXPECT_EQ(mul_winograd_multithread(B, A), D);
    EXPECT_EQ(mul_winograd_multithread(A, B, 2), C);
    EXPECT_EQ(mul_winograd_multithread(B, A, 2), D);
    EXPECT_EQ(mul_winograd_multithread(A, B, 4), C);
    EXPECT_EQ(mul_winograd_multithread(B, A, 4), D);
}


TEST(BigThreads, Equal)
{
    Matrix<12,13> A = { 2, 32, 4, 1, 3};
    Matrix<13,11> B = { 2, 3, -4, 1, 3};

    A[3][1] = 7;
    B[1][2] = 9;
    B[1][4] = 11;

    EXPECT_EQ(mul_winograd(A, B), mul_winograd_multithread(A, B, 5));
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
