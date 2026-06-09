/*
 * File: leaderboard_store.c
 * -------------------------
 * Serializes the top-10 leaderboard to/from SPI flash. See leaderboard_store.h
 * for the on-flash layout.
 */
#include "leaderboard_store.h"
#include "spi_flash.h"
#include <stdint.h>

#define FLASH_ADDR   0x000000        // first 4 KB sector
#define MAGIC        0xA57E0001u     // "ASTE" v1 -- marks a valid saved board
#define NUM_ENTRIES  10
#define NAME_LEN     4               // 3 letters + NUL
#define REC_SIZE     (4 + NAME_LEN)  // int32 score + name
#define BUF_SIZE     (4 + NUM_ENTRIES * REC_SIZE)   // magic + records = 84

// Home for the loaded name strings, since names[] are just pointers and the
// rest of the game expects them to stay valid for the whole session.
static char name_storage[NUM_ENTRIES][NAME_LEN];

static void put_u32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

static uint32_t get_u32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

bool leaderboard_load(int scores[10], char *names[10]) {
    spi_flash_init();
    if (spi_flash_jedec_id() != 0xEF4018) return false;   // chip absent/garbled

    uint8_t buf[BUF_SIZE];
    spi_flash_read(FLASH_ADDR, buf, BUF_SIZE);
    if (get_u32(buf) != MAGIC) return false;              // blank/unsaved flash

    int off = 4;
    for (int i = 0; i < NUM_ENTRIES; i++) {
        scores[i] = (int)get_u32(buf + off);
        off += 4;
        for (int j = 0; j < NAME_LEN; j++)
            name_storage[i][j] = (char)buf[off + j];
        name_storage[i][NAME_LEN - 1] = '\0';             // defensive NUL
        names[i] = name_storage[i];
        off += NAME_LEN;
    }
    return true;
}

void leaderboard_save(int scores[10], char *names[10]) {
    spi_flash_init();

    uint8_t buf[BUF_SIZE];
    put_u32(buf, MAGIC);

    int off = 4;
    for (int i = 0; i < NUM_ENTRIES; i++) {
        put_u32(buf + off, (uint32_t)scores[i]);
        off += 4;
        // Copy up to NAME_LEN bytes of the name, stopping (and zero-padding)
        // at its NUL so we never read past a short/empty string.
        const char *nm = names[i];
        bool done = false;
        for (int j = 0; j < NAME_LEN; j++) {
            char c = (!done && nm) ? nm[j] : '\0';
            buf[off + j] = (uint8_t)c;
            if (c == '\0') done = true;
        }
        off += NAME_LEN;
    }

    spi_flash_erase_4k(FLASH_ADDR);            // erase sector -> all 0xFF
    spi_flash_program(FLASH_ADDR, buf, BUF_SIZE);
}
