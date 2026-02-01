#ifndef HELPERS_H
#define HELPERS_H

#include "msp430fr2355.h"
#include <stdint.h>


/* Copies elements of *source into *dest.
 *
 * --- *source -> The array containing data to be copied.
 *
 * --- *dest ---> The array which data is copied into.
 *
 * --- count ---> The number of elements of *source to be copied
 *
 * */
void copyArray(uint8_t *source, uint8_t *dest, uint8_t count);


#endif
