
typedef unsigned short Bits16;
enum Exception {NO_EXCEPTION, NUMBER_FORMAT_EXCEPTION};

Bits16 Bits16_fromString(const char* value, Exception* exception);

char* Bits16_toString(Bits16 value);

/**
 * @param value The value for which to count the number of "1" bits.
 *
 * @return The number of "1" bits in the given value.
 */
size_t countOnes(Bits16 value);

void orSample();
void andSample();
void xorSample();
void notSample();
void shiftLeftSample();
