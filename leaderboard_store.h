#ifndef LEADERBOARD_STORE_H
#define LEADERBOARD_STORE_H

/*
 * Persists the top-10 leaderboard (scores + 3-letter names) to the W25Q128
 * SPI flash so it survives a power-off. Serializes into one 4 KB sector:
 *
 *     [0..3]   magic number (identifies a valid saved leaderboard)
 *     then 10 records of { int32 score; char name[4] }  -> 84 bytes total
 *
 * The names array holds char* pointers (into malloc'd buffers or static
 * storage); this module serializes the actual characters, not the pointers.
 */

#include <stdbool.h>

// Load the saved leaderboard from flash into the given arrays.
// Returns true if a valid saved leaderboard was found (arrays overwritten),
// false if the flash holds none yet (arrays left untouched -> keep defaults).
// On success, names[] are pointed at this module's static storage.
bool leaderboard_load(int scores[10], char *names[10]);

// Erase the sector and write the given leaderboard to flash. Blocks ~50-400ms.
// Call from main context, NOT an interrupt handler.
void leaderboard_save(int scores[10], char *names[10]);

#endif
