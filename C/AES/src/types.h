#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct _extendedKey_t { uint8_t data[11][4][4]; } extendedKey_t;
typedef struct _block16_t { uint8_t data[4][4]; } block16_t;

#endif

