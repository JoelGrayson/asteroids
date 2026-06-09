# Usage
# make run - main.c
# make tests/maths - tests asserts
# make tests/draw_line - draws on monitor
# make tests/audio - tests asteroids sounds playing through I2S


RUN_PROGRAM = main.bin

# Own custom implementations in lib/
LIBMANGO_SOURCES = libmango/gpio.o libmango/gpio_extra.o libmango/malloc.o libmango/ccu.o libmango/i2s.o libmango/dma.o libmango/printf.o
GRAPHICS_SOURCES = graphics/draw_line.o graphics/draw_points.o graphics/geometry.o graphics/rotate_points.o graphics/rotate_vector.o #graphics/draw_saucer.o

OTHER_SOURCES = maths.o asteroid.o rocket.o mechanics.o buttons.o bullets.o explosion.o score_and_lives.o audio/sounds.o saucer.o start_game_screen.o game_over_screen.o collision_detection.o frame.o

run: $(RUN_PROGRAM)
	mango-run $<

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

# --allow-multiple-definition: our own libmango/gpio_extra.o defines the same
# pull-state symbols that the staff -lmango archive does. Listing our objects
# before -lmango means ld keeps our definitions and ignores the duplicates.
LD_FLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld --allow-multiple-definition
LDLIBS = -lmango -lmango_gcc
%.elf: %.o $(LIBMANGO_SOURCES) $(GRAPHICS_SOURCES) $(OTHER_SOURCES)
	riscv64-unknown-elf-ld $(LD_FLAGS) $^ $(LDLIBS) -o $@

ARCH = -march=rv64im_zicsr -mabi=lp64 
CFLAGS = $(ARCH) -g -Og -I$$CS107E/include -fno-omit-frame-pointer $$warn $$freestanding -fstack-protector-strong -Wno-builtin-declaration-mismatch  #no-builtin-decoration-mismatch because we implement math functions with different type signatures
%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@


clean:
	find . -name '*.o' -delete
	find . -name '*.bin' -delete
	find . -name '*.elf' -delete
	# in bash, rm **/*.o matches not current directory


tests/maths_test: tests/maths_test.bin
	mango-run $<

tests/draw_line_test: tests/draw_line_test.bin
	mango-run $<


tests/draw_asteroid_test: tests/draw_asteroid_test.bin
	mango-run $<

tests/audio_test: tests/audio_test.bin
	mango-run $<



# Self-contained bring-up test: links only against the staff -lmango library
# (gpio/uart/timer/printf), NOT the game objects. This explicit rule overrides
# the generic %.elf rule above so we don't drag in game_over_screen.o etc.
tests/jedec_test.elf: tests/jedec_test.o
	riscv64-unknown-elf-ld $(LD_FLAGS) $^ $(LDLIBS) -o $@

tests/jedec_test: tests/jedec_test.bin
	mango-run $<

# Write-path test: links the test object + the spi_flash driver against the
# staff -lmango library only (no game objects). Explicit rule overrides %.elf.
tests/flash_rw_test.elf: tests/flash_rw_test.o spi_flash.o
	riscv64-unknown-elf-ld $(LD_FLAGS) $^ $(LDLIBS) -o $@

tests/flash_rw_test: tests/flash_rw_test.bin
	mango-run $<



# .PRECIOUS: %.o %.elf %.bin



export warn = -Wall -Wpointer-arith -Wwrite-strings -Werror \
              -Wno-unused-function -Wno-error=unused-variable \
              -fno-diagnostics-show-option
export freestanding = -ffreestanding -nostdinc \
                      -isystem $(shell riscv64-unknown-elf-gcc -print-file-name=include)

