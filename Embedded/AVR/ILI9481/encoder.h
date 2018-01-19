#ifndef __ENCODER_H__
#define __ENCODER_H__

/**
 * Initializes the encoder unit.
 */
void encoderInit();

/**
 * Reads the current delta of encoder 1.
 * Also clears the encoder's current delta.
 *
 * @return The encoder delta since the last read (-128 ... 127)
 */
int8_t encoderRead1();

/**
 * Reads the current delta of encoder 2.
 * Also clears the encoder's current delta.
 *
 * @return The encoder delta since the last read (-128 ... 127)
 */
int8_t encoderRead2();

#endif
