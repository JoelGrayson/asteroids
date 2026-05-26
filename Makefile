# Usage
# make run - main.c
# make tests/maths - tests asserts
# make tests/draw_line - draws on monitor



RUN_PROGRAM = main.bin

# Own custom implementations in lib/
LIBMANGO_SOURCES = libmango/gpio.o libmango/malloc.o #TODO: libmango/printf.o
GRAPHICS_SOURCES = graphics/draw_line.o graphics/draw_points.o graphics/draw_saucer.o
OTHER_SOURCES = maths.o asteroid.o


run: $(RUN_PROGRAM)
	mango-run $<

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

LD_FLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
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


# .PRECIOUS: %.o %.elf %.bin



export warn = -Wall -Wpointer-arith -Wwrite-strings -Werror \
              -Wno-unused-function -Wno-error=unused-variable \
              -fno-diagnostics-show-option
export freestanding = -ffreestanding -nostdinc \
                      -isystem $(shell riscv64-unknown-elf-gcc -print-file-name=include)

