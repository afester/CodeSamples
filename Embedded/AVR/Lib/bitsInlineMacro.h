#ifndef __BITSINLINEMACRO_H__
#define __BITSINLINEMACRO_H__

/* C Inline functions wrapped in macros to avoid the need for & at the calling site */
inline void _SETBIT(volatile uint8_t *port, uint8_t bit)    { *port |= _BV(bit); }
inline void _CLEARBIT(volatile uint8_t *port, uint8_t bit)  { *port &= ~_BV(bit); }
inline void _TOGGLEBIT(volatile uint8_t *port, uint8_t bit) { *port ^= _BV(bit); }

#define bitSet(port, bit) (_SETBIT(&port, bit))
#define bitClear(port, bit) (_CLEARBIT(&port, bit))
#define bitToggle(port, bit) (_TOGGLEBIT(&port, bit))

#endif
