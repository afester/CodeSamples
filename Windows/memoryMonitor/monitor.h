#include <stdint.h>

/**
 * Function pointer type for the memory observer
 */
typedef void(*omObserver_t)(uint8_t* addr, uint8_t value);

/**
 * Creates an area of observed memory.
 *
 * Currently, only one page of observed memory is supported.
 *
 * @param observer A pointer to a function which is called each time before the
 *        new value is written into the observed memory area.
 */
uint8_t *omAllocate(omObserver_t observer);
