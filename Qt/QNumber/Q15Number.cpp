/*
 * Q15Number.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: andreas
 */

#include "Q15Number.h"
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

Q15Number::Q15Number() {
    value = 0;

}

Q15Number::Q15Number(float Float_Value) {
    // convert from float: multiply signed integer with 2^15 and round to nearest integer
    value =
        ((Float_Value < 0.0) ? (signed int)(32768 * (Float_Value) - 0.5)
                             : (signed int)(32767 * (Float_Value) + 0.5));
}

Q15Number::~Q15Number() {
}

Q15Number Q15Number::operator+(const Q15Number& val2) {
    // Add two Q15 numbers: simply add the integer values
    value = value + val2.value;
    return *this;
}

float Q15Number::toFloat() const {
    // Convert to float: take signed integer value and multiply with 2^-15
    return value * 3.0517578125e-05;
}

typedef signed short Q15;
const int MAX_FRAC_LENGTH = 10;

void fracToString(Q15 value, char* result) {
    int remainder = value;
    unsigned char digit = 0;
    short idx = 0;

    if (remainder < 0) {
        result[idx++] = '-';
        remainder = -remainder;
    }

    digit = remainder >> 15;        // / 32768;
    remainder = remainder & 0x7FFF; // % 32768;
    result[idx++] = '0' + digit;

    if (remainder) {
        result[idx++] = '.';

        for (int i = 0;  i < 7 && remainder;  i++) {
            remainder = remainder * 10;     // potential OVERFLOW!

            digit = remainder >> 15;        // / 32768;
            remainder = remainder & 0x7FFF; // % 32768;
            result[idx++] = '0' + digit;
        }
    }
    result[idx++] = 0;
}


string Q15Number::asString() const {
    char buffer[MAX_FRAC_LENGTH + 1];
    fracToString(value, buffer);
    return buffer;
}




std::string Q15Number::toString() const {
    std::stringstream result;

    result << "Q15Number[value=0x" << setfill('0')<< setw(4) << hex << value <<
                        ", float=" << toFloat() <<
                        ", asString=" << dec << asString() << "]";

    return result.str();
}


Q15Number Q15Number::divide(short val1, short val2) {
    return Q15Number((float) val1 / val2);
}

int main() {

    std::cerr << "\"Random\" numbers:" << endl;
    Q15Number val1(0.01);
    Q15Number val2(0.95);
    Q15Number val3(-0.45);
    std::cerr << "   " << val1.toString() << std::endl;
    std::cerr << "   " << val2.toString() << std::endl;
    std::cerr << "   " << val3.toString() << std::endl;

    std::cerr << endl << "Arithmetics:" << endl;
    Q15Number sum = val1 + val2;
    std::cerr << "   " << "Sum:" << sum.toString() << std::endl;
    Q15Number quotient = Q15Number::divide(4096, 16384);
    std::cerr << "   " << "Quotient of two int's: "<< quotient.toString() << std::endl;

    // extreme numbers
    std::cerr << endl << "Extreme numbers:" << endl;
    Q15Number val4(-1);
    std::cerr << "   " << val4.toString() << std::endl;
    Q15Number val5(0);
    std::cerr << "   " << val5.toString() << std::endl;
    Q15Number val6(0.99999);  // 0.999969482
    std::cerr << "   " << val6.toString() << std::endl;
}
