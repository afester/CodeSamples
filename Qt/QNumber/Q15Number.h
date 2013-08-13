/*
 * Q15Number.h
 *
 *  Created on: Jun 14, 2011
 *      Author: andreas
 */

#ifndef Q15NUMBER_H_
#define Q15NUMBER_H_

#include <string>


class Q15Number {
    signed short value;

public:
    Q15Number();

    Q15Number(float val);

    virtual ~Q15Number();

    virtual float toFloat() const;

    virtual std::string asString() const;

    virtual std::string toString() const;

    virtual Q15Number operator+(const Q15Number& val2);

    static Q15Number divide(short val1, short val2);
};

#endif /* Q15NUMBER_H_ */
