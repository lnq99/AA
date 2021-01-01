#include "matrix/matrix.h"
#include <iostream>


int main(int argc, char** argv)
{
    Matrix<2,3> A;
    Matrix<2,3> B = {
        1, 2, 3,
        3, 4, 5
    };

    try
    {
        auto C = A * B;
        std::cout << C;
    }
    catch (std::exception &e)
    {
        std::cout << "A  " << A << "B  " << B;
        std::cout << e.what();
        std::cout << " - Can't multiply matrices\n\n";
    }


    Matrix<2,3, int8_t> C = {
        2,4,3,
        1,-3,2
    };
    Matrix<3,2, int8_t> D = {
        2,-3,
        4,4,
        2,3
    };

    std::cout << "==================\n";
    std::cout << "C  " << C << "D  " << D;

    std::cout << "Standard algorithm\n";
    std::cout << "CxD" << C * D;
    std::cout << "DxC" << D * C;

    std::cout << "Coppersmith–Winograd algorithm\n";
    std::cout << "CxD" << (C ^ D);
    std::cout << (D ^ C);

    std::cout << "Coppersmith–Winograd algorithm optimized\n";
    std::cout << "CxD" << (C & D);
    std::cout << (D & C);

    return 0;
}