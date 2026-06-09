#ifndef SPI_FLASH_H
#define SPI_FLASH_H

/*
 * Minimal bit-banged SPI driver for the Winbond W25Q128 NOR flash chip.
 *
 * SPI mode 0, single-bit. Pins are defined at the top of spi_flash.c and
 * must match the physical wiring (and tests/jedec_test.c):
 *     CS=PE16  SCLK=PB2  MOSI/DI=PB3  MISO/DO=PC0
 *
 * Remember the flash write model:
 *   - Programming can only flip bits 1 -> 0. To store arbitrary bytes you
 *     must erase first (erase sets a region back to all 0xFF).
 *   - The smallest erasable unit is a 4 KB sector.
 *   - So the save pattern is: spi_flash_erase_4k(addr); spi_flash_program(...).
 * Write-enable and BUSY-polling are handled internally by erase/program.
 */

#include <stdint.h>
#include <stddef.h>

// Initialize the GPIO pins (CS high, clock low). Call once before use.
void spi_flash_init(void);

// Read the 3-byte JEDEC ID. A genuine W25Q128 returns 0xEF4018.
uint32_t spi_flash_jedec_id(void);

// Read `n` bytes starting at `addr` into `buf`.
void spi_flash_read(uint32_t addr, uint8_t *buf, size_t n);

// Erase the 4 KB sector containing `addr` (sets those 4 KB to 0xFF).
// Blocks until the chip reports the erase is complete (~45-400 ms).
void spi_flash_erase_4k(uint32_t addr);

// Program (write) `n` bytes from `buf` to `addr`. `n` must be <= 256 and the
// write must stay within a single 256-byte page (addr/256 == (addr+n-1)/256);
// the sector must have been erased first. Blocks until complete (~1-3 ms).
void spi_flash_program(uint32_t addr, const uint8_t *buf, size_t n);

#endif
