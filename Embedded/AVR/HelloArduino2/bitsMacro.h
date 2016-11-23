#ifndef __BITSMACRO_H__
#define __BITSMACRO_H__

/* Macros */
#define bitSet(port, bit)    ((port) |= _BV(bit))
#define bitClear(port, bit)  ((port) &= ~_BV(bit))
#define bitToggle(port, bit) ((port) ^= _BV(bit))

#endif
