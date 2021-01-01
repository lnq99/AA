#ifndef BASE_MATRIX_H
#define BASE_MATRIX_H

#include <iostream>


class BaseMatrix
{
protected:
    size_t row, col;

public:
    BaseMatrix(size_t r, size_t c) : row(r), col(c) {};
    ~BaseMatrix() {};

    size_t n_row() const { return row; }
    size_t n_col() const { return col; }

    virtual std::ostream& display(std::ostream& os) const = 0;
};


std::ostream& operator<<(std::ostream& os, const BaseMatrix& m)
{
    os << "[" << m.n_row() << "x" << m.n_col() << "]\n";
    m.display(os);
    os << '\n';
    return os;
}


#endif
