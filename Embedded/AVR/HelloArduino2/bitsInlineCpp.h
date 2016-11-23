#ifndef __BITSINLINECPP_H__
#define __BITSINLINECPP_H__

/* C++ inline functions */
inline void bitSet(volatile uint8_t& port, uint8_t bit)    { port |= _BV(bit); }
inline void bitClear(volatile uint8_t& port, uint8_t bit)  { port &= ~_BV(bit); }
inline void bitToggle(volatile uint8_t& port, uint8_t bit) { port ^= _BV(bit); }

#endif
