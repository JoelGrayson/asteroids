# Usage
# make run - main.c
# make tests/maths - tests asserts
# make tests/draw_line - draws on monitor



RUN_PROGRAM = main.bin

# Own custom implementations in lib/
LIB_SOURCES = libmango/printf.o libmango/gpio.o libmango/malloc.o


run: $(RUN_PROGRAM)
	mango-run $<

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

LD_FLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS = -lmango -lmango_gcc
%.elf: %.o $(LIB_SOURCES) maths.o
	riscv64-unknown-elf-ld $(LD_FLAGS) $^ $(LDLIBS) -o $@ 

ARCH = -march=rv64im_zicsr -mabi=lp64 
CFLAGS = $(ARCH) -g -Og -I$$CS107E/include -fno-omit-frame-pointer $$warn $$freestanding -fstack-protector-strong -Wno-builtin-declaration-mismatch  #no-builtin-decoration-mismatch because we implement math functions with different type signatures
%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@


clean: #-f for it to shut up
	rm -f **/*.o **/*.bin **/*.elf


tests/maths_test: tests/maths.bin
	mango-run $<

tests/draw_line_test: tests/draw_line.bin
	mango-run $<


.PRECIOUS: %.o %.elf %.bin



export warn = -Wall -Wpointer-arith -Wwrite-strings -Werror \
              -Wno-unused-function -Wno-error=unused-variable \
              -fno-diagnostics-show-option
export freestanding = -ffreestanding -nostdinc \
                      -isystem $(shell riscv64-unknown-elf-gcc -print-file-name=include)
# export freestanding = -ffreestanding -nostdinc

