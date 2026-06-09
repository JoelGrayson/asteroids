/*
 * File: spi_flash.c
 * -----------------
 * Bit-banged SPI mode-0 driver for the Winbond W25Q128 NOR flash.
 * See spi_flash.h for the API and the flash write model.
 */
#include "spi_flash.h"
#include "gpio.h"
#include "timer.h"

// >>> Must match the physical wiring (and tests/jedec_test.c). <<<
#define PIN_CS    GPIO_PE16   // CS  / CE#
#define PIN_SCLK  GPIO_PB2    // CLK / SCK
#define PIN_MOSI  GPIO_PB3    // DI  (data into the flash)
#define PIN_MISO  GPIO_PC0    // DO  (data out of the flash)

// W25Qxx command opcodes
#define CMD_JEDEC 0x9f
#define CMD_WREN  0x06   // write enable (sets WEL latch)
#define CMD_RDSR1 0x05   // read status register 1
#define CMD_READ  0x03   // read data
#define CMD_PP    0x02   // page program (write)
#define CMD_SE    0x20   // sector erase (4 KB)

#define SR1_BUSY  0x01   // status register 1, bit 0: erase/program in progress

// One microsecond per half-cycle is far more than the chip needs; it just
// keeps the bit-banged edges clean over breadboard/jumper wiring.
static inline void dly(void) { timer_delay_us(1); }

// Shift one byte out on MOSI while shifting one in on MISO (SPI mode 0:
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

static void cs_low(void)  { gpio_write(PIN_CS, 0); dly(); }
static void cs_high(void) { gpio_write(PIN_CS, 1); dly(); }

// Send a 24-bit address, most-significant byte first.
static void send_addr(uint32_t a) {
    xfer((a >> 16) & 0xff);
    xfer((a >> 8) & 0xff);
    xfer(a & 0xff);
}

// Set the write-enable latch. Required before every erase or program;
// the chip clears it automatically once the operation completes.
static void write_enable(void) {
    cs_low();
    xfer(CMD_WREN);
    cs_high();
}

// Poll status register 1 until the BUSY bit clears (erase/program finished).
static void wait_until_ready(void) {
    cs_low();
    xfer(CMD_RDSR1);
    while (xfer(0xff) & SR1_BUSY)
        ;
    cs_high();
}

void spi_flash_init(void) {
    gpio_set_output(PIN_CS);
    gpio_set_output(PIN_SCLK);
    gpio_set_output(PIN_MOSI);
    gpio_set_input(PIN_MISO);
    gpio_write(PIN_CS, 1);     // CS idle high (deselected)
    gpio_write(PIN_SCLK, 0);   // clock idle low (mode 0)
}

uint32_t spi_flash_jedec_id(void) {
    cs_low();
    xfer(CMD_JEDEC);
    uint32_t id = (uint32_t)xfer(0xff) << 16;
    id |= (uint32_t)xfer(0xff) << 8;
    id |= xfer(0xff);
    cs_high();
    return id;
}

void spi_flash_read(uint32_t addr, uint8_t *buf, size_t n) {
    cs_low();
    xfer(CMD_READ);
    send_addr(addr);
    for (size_t i = 0; i < n; i++)
        buf[i] = xfer(0xff);
    cs_high();
}

void spi_flash_erase_4k(uint32_t addr) {
    write_enable();
    cs_low();
    xfer(CMD_SE);
    send_addr(addr);
    cs_high();
    wait_until_ready();
}

void spi_flash_program(uint32_t addr, const uint8_t *buf, size_t n) {
    write_enable();
    cs_low();
    xfer(CMD_PP);
    send_addr(addr);
    for (size_t i = 0; i < n; i++)
        xfer(buf[i]);
    cs_high();
    wait_until_ready();
}
