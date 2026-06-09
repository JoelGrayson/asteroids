/* File: tests/jedec_test.c
 * ------------------------
 * Bring-up test for the W25Q128 SPI NOR flash chip.
 *
 * Bit-bangs SPI mode 0 to send the Read-JEDEC-ID command (0x9F) and prints
 * the three ID bytes over UART. A genuine Winbond W25Q128 answers with
 *     manufacturer=0xEF  type=0x40  capacity=0x18   -> id 0xEF4018
 *
 * This test is deliberately SELF-CONTAINED (it does not depend on spi_flash.c)
 * so you can verify the wiring is correct before building the full driver.
 *
 * Run with:  make tests/jedec_test
 */
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "printf.h"
#include <stdint.h>

#define PIN_CS    GPIO_PE16   // module CS  / CE#
#define PIN_SCLK  GPIO_PB2    // module CLK / SCK
#define PIN_MOSI  GPIO_PB3    // module DI  (data into the flash)
#define PIN_MISO  GPIO_PC0    // module DO  (data out of the flash)

// One microsecond is far more than the chip needs; it just keeps the
// bit-banged edges clean over breadboard/jumper wiring.
static inline void dly(void) { timer_delay_us(1); }

// Shift one byte out on MOSI while shifting one byte in on MISO (SPI mode 0:
// clock idles low, MOSI set while clock low, both ends sample on rising edge).
static uint8_t xfer(uint8_t out) {
    uint8_t in = 0;
    for (int i = 7; i >= 0; i--) {
        gpio_write(PIN_MOSI, (out >> i) & 1);     // present bit while clock low
        dly();
        gpio_write(PIN_SCLK, 1);                  // rising edge: chip samples MOSI...
        in = (in << 1) | gpio_read(PIN_MISO);     // ...and we sample MISO
        dly();
        gpio_write(PIN_SCLK, 0);                  // falling edge
    }
    return in;
}

int main(void) {
    uart_init();
    printf("\n==== W25Q128 JEDEC ID test ====\n");

    // Idle state: CS high (deselected), clock low (mode 0).
    gpio_set_output(PIN_CS);
    gpio_set_output(PIN_SCLK);
    gpio_set_output(PIN_MOSI);
    gpio_set_input(PIN_MISO);
    gpio_write(PIN_CS, 1);
    gpio_write(PIN_SCLK, 0);
    timer_delay_ms(10);                           // let the chip power-settle

    // Read JEDEC ID: drop CS, send 0x9F, clock out 3 bytes, raise CS.
    gpio_write(PIN_CS, 0);
    dly();
    xfer(0x9f);
    uint8_t mf  = xfer(0xff);                      // manufacturer
    uint8_t typ = xfer(0xff);                      // memory type
    uint8_t cap = xfer(0xff);                      // capacity
    gpio_write(PIN_CS, 1);

    uint32_t id = ((uint32_t)mf << 16) | ((uint32_t)typ << 8) | cap;
    printf("JEDEC ID = 0x%06x   (mf=0x%02x  type=0x%02x  cap=0x%02x)\n",
           id, mf, typ, cap);

    if (id == 0xEF4018) {
        printf("PASS: genuine Winbond W25Q128 detected.\n");
    } else if (mf == 0xEF) {
        printf("Winbond chip, but not a W25Q128 (different density?).\n");
    } else if (id == 0x000000 || id == 0xFFFFFF) {
        printf("FAIL: no response from chip. Check:\n");
        printf("  - WP# and HOLD# tied to 3.3V (NOT left floating)\n");
        printf("  - VCC = 3.3V and a common ground with the Pi\n");
        printf("  - DI/DO (MOSI/MISO) not swapped\n");
        printf("  - the PIN_* #defines above match your actual wiring\n");
    } else {
        printf("FAIL: unexpected ID -- likely a wiring or timing issue.\n");
    }

    while (1) { /* SPIN so the message stays on screen */ }
}
