#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;


void hexprint(const void* array, size_t size) {
   const unsigned char* data = (const unsigned char*) array;
   for(int i = 0;  i < size;  i++) {
      printf("%02x ", data[i]);
   }
}


/**
 * Simple class to analyze and dump a floating point value.
 */
class PureFloat {

   static const unsigned int BIT24 = 0x00800000;

   union FloatUni {
      float f;
      struct {
         unsigned int m : 23;	/* mantisse */
         unsigned int e : 8;	/* exponent */
         unsigned int s : 1;	/* sign */
      } b;
   };

   FloatUni val;

public:
    PureFloat(float value = 0.0);

    PureFloat(int e, int m);

    float value() const;

    /**
     * @return The biased (original) exponent.
     */
    int e() const;

    /**
     * @return The normalized mantissa (means, with the 24th bit set)
     */
    unsigned int m() const;

    static PureFloat normalized(unsigned int resultMant, int newExp);

    /**
     * Prints out information about the current float value, such as 
     * the exponent and the mantissa
     */
    void dumpInfo() const;
    
    /**
     * Prints out the current floating point number in scientific
     * notation, using two lines
     */
    void dumpScientific() const;

    /**
     * Prints out the current floating point number in a binary
     * format, showing the IEEE754 format bit by bit
     */
    void dumpBinary() const;

    /**
     * @return A string representaion of this floating point number.
     */
    string toString() const;

    /**
     * Adds two floating point numbers.
     */
    friend PureFloat operator+(const PureFloat& s1, const PureFloat& s2);

    /**
     * Subtracts two floating point numbers.
     */
    friend PureFloat operator-(const PureFloat& s1, const PureFloat& s2);

    /**
     * Multiplies two floating point numbers.
     */
    friend PureFloat operator*(const PureFloat& s1, const PureFloat& s2);

    /**
     * Divides two floating point numbers.
     */
    friend PureFloat operator/(const PureFloat& s1, const PureFloat& s2);
};


PureFloat::PureFloat(float aFloat) {
    val.f = aFloat;
}

PureFloat::PureFloat(int e, int m) {
    val.b.s = 0; // TODO!
    val.b.e = e;
    val.b.m = m;
}

float PureFloat::value() const {
    return val.f;
}

int PureFloat::e() const {
    return val.b.e;
}

unsigned int PureFloat::m() const {
    return val.b.m | PureFloat::BIT24;
}

void PureFloat::dumpInfo() const {
    printf("  Value: %f\n", val.f);
    printf("  Dump : ");
    hexprint(&val.f, sizeof(val.f));
    printf("\n  Sign : %d\n", val.b.s);
    printf("  Exp  : %d\n", val.b.e);
    printf("  Mant.: %d\n", val.b.m);
    printf("                       %d-127\n", val.b.e);
    printf("  %c(1 + %f) * 2\n",  val.b.s ? '-' : ' ', (float) val.b.m * 0.00000011920928955078125);
    printf("= %c(1 + %f) * %d\n", val.b.s ? '-' : ' ', (float) val.b.m * 0.00000011920928955078125, 1 << (val.b.e - 127));
    printf("= %c%f\n",      val.b.s ? '-' : ' ', (float) (1.0 + val.b.m * 0.00000011920928955078125) * (1 << (val.b.e - 127)));
}


void PureFloat::dumpScientific() const {
    printf("                       %d-127\n", val.b.e);
    printf("  %c%f * 2\n",  val.b.s ? '-' : ' ', 1.0+val.b.m * 0.00000011920928955078125);
}


void PureFloat::dumpBinary() const {
    printf("s=%d e=%d m=%d\n", val.b.s, val.b.e, val.b.m);
    printf("+-+--------+-----------------------+\n");
    printf("|%d|", val.b.s);

    unsigned int mask = 0x0080; 
    for (int b = 0;  b < 8;  b++, mask >>= 1) {
        printf("%d", (val.b.e & mask) != 0);
    }
    printf("|");

    mask = 0x400000; 
    for (int b = 0;  b < 23;  b++, mask >>= 1) {
        printf("%d", (val.b.m & mask) != 0);
    }

    printf("|\n+-+--------+-----------------------+\n\n");
}


PureFloat PureFloat::normalized(unsigned int resultMant, int newExp) {
    while( (resultMant & 0xFF000000) != 0) {
        newExp++;
	resultMant >>= 1;
    }
    return PureFloat(newExp, resultMant & 0x007FFFFF);
}

/* itoa is non-standard and non-portable! */
static string pitoa(int number) {
   return static_cast<ostringstream*>( &(ostringstream() << number) )->str();
}

// http://www.ragestorm.net/blogs/?p=57
// Base algorithm to convert an IEEE754 floating point number into a string.
// very small numbers and very large numbers are not supported yet.
string PureFloat::toString() const {
    string result = "";

    // 1. Get sign, exponent and (normalized) mantissa
    unsigned int sign = val.b.s;
    unsigned int exp = val.b.e - 127;
    unsigned int man = val.b.m;
    man |= BIT24;

    // 2. Start with minus sign if value is negative
    if (sign) {
        result = "-";
    }

    unsigned int number = 0;

    // 3. Convert the integer part
    number = man >> (23 - exp);
    result.append(pitoa(number));

    // 4. Convert the fractional part
    unsigned int frac = man & ((1 << (23-exp)) - 1);
    if (frac != 0) {
        result.append(".");

        unsigned int base = 1 << (23 - exp);
        int c = 0;
        while (frac != 0 && c++ < 6) {
            frac *= 10;
            number = frac / base;
            result.append(pitoa(number));
            frac %= base;
        }
    }

    return result;
}


PureFloat operator+(const PureFloat& s1, const PureFloat& s2) {
    PureFloat X = s1;
    PureFloat Y = s2;

    // Make sure that the exponent in Y is less than or equal to the exponent in X
    if (s2.e() > s1.e()) {
        X = s2;
        Y = s1;
    }

    // Step 1: calculate same number as s2, but with same exponent as s1,
    // by adjusting the mantissa and the exponent
    int eDiff = X.e() - Y.e();
    int newExp = Y.e() + eDiff;
    unsigned int newMant = Y.m() >> eDiff;

    // Step 2: add the mantissa from s1 to the adjusted mantissa of s2
    unsigned int resultMant = X.m() + newMant;

    // Step 3: create the result by normalizing the result mantissa and exponent
    PureFloat result = PureFloat::normalized(resultMant, newExp);

    return result;
}


PureFloat operator-(const PureFloat& s1, const PureFloat& s2) {
    PureFloat X = s1;
    PureFloat Y = s2;

    // Make sure that the exponent in Y is less than or equal to the exponent in X
    if (s2.e() > s1.e()) {
        X = s2;
        Y = s1;
    }

    // Step 1: calculate same number as s2, but with same exponent as s1,
    // by adjusting the mantissa and the exponent
    int eDiff = X.e() - Y.e();
    int newExp = Y.e() + eDiff;
    unsigned int newMant = Y.m() >> eDiff;

    // Step 2: add the mantissa from s1 to the adjusted mantissa of s2
    unsigned int resultMant = X.m() - newMant;
    
    // Step 3: create the result by normalizing the result mantissa and exponent
    PureFloat result = PureFloat::normalized(resultMant, newExp);

    return result;
}


PureFloat operator*(const PureFloat& s1, const PureFloat& s2) {
    // First, convert the two representations to scientific notation. Thus, we explicitly represent the hidden 1. 
    unsigned int Xm = s1.m();
    unsigned int Ym = s2.m();
    
    // Let x be the exponent of X. Let y be the exponent of Y. 
    // The resulting exponent (call it z) is the sum of the two exponents. z may need to be adjusted after the next step. 
    int z = ((s1.e() - 127) + (s2.e() - 127)) + 127;

    // Multiply the mantissa of X to the mantissa of Y. Call this result m. 
    unsigned int m = ((unsigned long long) Xm * Ym) >> 23;

    // If m is does not have a single 1 left of the radix point, then adjust the radix point so it does, and adjust the exponent z to compensate. 
    PureFloat result = PureFloat::normalized(m, z);

    // Add the sign bits, mod 2, to get the sign of the resulting multiplication.

    // Convert back to the one byte floating point representation, truncating bits if needed.

    return result;

}


PureFloat operator/(const PureFloat& s1, const PureFloat& s2) {
    // First, convert the two representations to scientific notation. Thus, we explicitly represent the hidden 1. 
    unsigned int Xm = s1.m();
    unsigned int Ym = s2.m();
    
    // Let x be the exponent of X. Let y be the exponent of Y. 
    // The resulting exponent (call it z) is the sum of the two exponents. z may need to be adjusted after the next step. 
    int z = ((s1.e() - 127) - (s2.e() - 127)) + 127;

    // divide  the mantissa of X to the mantissa of Y. Call this result m. 
    unsigned int m = ((unsigned long long) Xm / Ym) >> 23;

    // If m is does not have a single 1 left of the radix point, then adjust the radix point so it does, and adjust the exponent z to compensate. 
    PureFloat result = PureFloat::normalized(m, z);

    // Add the sign bits, mod 2, to get the sign of the resulting multiplication.

    // Convert back to the one byte floating point representation, truncating bits if needed.

    return result;
}


ostream& operator<<(ostream& o, const PureFloat& pf) {
    o << pf.toString();
    return o;
}


int main1() {
    PureFloat f1(42.987654321);
    PureFloat f2(1.0);
    PureFloat f3(2.21);
    PureFloat f4(155.43);
    PureFloat f5(123.234);
    PureFloat f6(0.05);
    PureFloat f7(2.0);

    string str = f1.toString();
    cerr << str << endl;

    PureFloat res = f1 + f2;
    std::cerr << f1 << " + " << f2 << " = " << res << std::endl;

    res = f4 + f3;
    std::cerr << f4 << " + " << f3 << " = " << res << std::endl;

    res = f3 + f4;
    std::cerr << f3 << " + " << f4 << " = " << res << std::endl;

    res = f5 + f6;
    std::cerr << f5 << " + " << f6 << " = " << res << std::endl;
    
    res = f5 - f6;
    std::cerr << f5 << " - " << f6 << " = " << res << std::endl;

    res = f3 * f3;
    std::cerr << f3 << " * " << f3 << " = " << res << std::endl;

    res = f5 * f5;
    std::cerr << f5 << " * " << f5 << " = " << res << std::endl;

    res = f2 / f7;
    std::cerr << f2 << " / " << f7 << " = " << res << std::endl;
    f3.dumpInfo();
}


int main() {
    PureFloat f1(0.0);
    f1.dumpBinary();

    PureFloat f2(1.0);
    f2.dumpBinary();

    PureFloat f3(40.0);
    f3.dumpInfo();

    PureFloat f4(-0.1);
    f4.dumpBinary();
    printf("%d\n", f4.value() == f4.value());

    PureFloat f5(NAN);
    f5.dumpBinary();
    printf("%d\n", f5.value() == f5.value());

    double d = 4.0;
    printf("len: %d\n", sizeof(d));
    hexprint((void*) &d, sizeof(d));
}
