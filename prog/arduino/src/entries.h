#ifndef _ENTRIES_H_
#define _ENTRIES_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#define read_entry(e, i, j) (pgm_read_byte(&(e[i][j])))

#include "entries_example.h"

#endif /* _ENTRIES_H_ */
