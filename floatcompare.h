#ifndef FL_CMP_H
#define FL_CMP_H

#include <cmath>
#include <limits>

bool fl_equal(float a, float b) {
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}

bool dbl_equal(double a, double b) {
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

#endif // FL_CMP_H
