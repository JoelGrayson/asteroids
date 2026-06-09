/* File: tests/flash_rw_test.c
 * ---------------------------
 * Write-path bring-up: a persistent boot counter stored in SPI flash.
 *
 * On each boot it reads a 4-byte counter from flash, prints it, then
 * increments it and writes it back (erase sector, then program). Power-cycle
 * the board and the number should climb by one every boot -- which proves
 * BOTH that writes work AND that the data survives a power-off, exactly what
 * the leaderboard needs.
 *
 * It also does an immediate read-back-and-verify so a failure shows up on the
 * first run, not only after a power cycle.
 *
 * Run with:  make tests/flash_rw_test
 */
#include "../spi_flash.h"
#include "uart.h"
#include "printf.h"
#include <stdint.h>

#define COUNTER_ADDR 0x000000   // first 4 KB sector, reserved for this test

static uint32_t read_u32(uint32_t addr) {
    uint8_t b[4];
    spi_flash_read(addr, b, 4);
    return (uint32_t)b[0] | ((uint32_t)b[1] << 8) |
           ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
}

static void write_u32(uint32_t addr, uint32_t v) {
    uint8_t b[4] = { (uint8_t)v, (uint8_t)(v >> 8),
                     (uint8_t)(v >> 16), (uint8_t)(v >> 24) };
    spi_flash_erase_4k(addr);       // erase 4 KB sector -> all 0xFF
    spi_flash_program(addr, b, 4);  // then program our 4 bytes
}

int main(void) {
    uart_init();
    spi_flash_init();

    printf("\n==== SPI flash boot-counter test ====\n");

    uint32_t id = spi_flash_jedec_id();
    printf("JEDEC ID = 0x%06x\n", id);
    if (id != 0xEF4018) {
        printf("FAIL: flash not detected -- fix wiring before testing writes.\n");
        while (1) ;
    }

    uint32_t count = read_u32(COUNTER_ADDR);
    if (count == 0xFFFFFFFF) {       // a blank/erased sector reads as all-ones
        printf("First boot (flash blank) -- starting count at 0.\n");
        count = 0;
    }
    printf("Boot count read from flash: %d\n", count);

    uint32_t next = count + 1;
    write_u32(COUNTER_ADDR, next);
    uint32_t check = read_u32(COUNTER_ADDR);

    if (check == next) {
        printf("Wrote %d and read it back OK.\n", next);
        printf("PASS: power-cycle the board -- this number should rise by 1 each boot.\n");
    } else {
        printf("FAIL: wrote %d but read back %d.\n", next, check);
        printf("  - is WP# tied to 3.3V? (writes are blocked if WP# is low/floating)\n");
        printf("  - erase/program BUSY-poll timing\n");
    }

    while (1) { /* SPIN so the message stays on screen */ }
}
