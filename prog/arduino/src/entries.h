#ifndef _ENTRIES_H_
#define _ENTRIES_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#define read_entry_time(e, i) (pgm_read_dword(&(e[i])))
#define read_entry_direction(e, i) (pgm_read_byte(&(e[i])))
#define read_entry_degrees(e, i) (pgm_read_byte(&(e[i])))

#include "entries_example.h"
#include "entries_WhiteGlint.h"
#include "entries_mission3.h"

#endif /* _ENTRIES_H_ */
