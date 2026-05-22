RUN_PROGRAM = main.bin

# Own custom implementations in lib/
LIB_SOURCES = lib/printf.o # lib/malloc.o lib/gpio.o


run: $(RUN_PROGRAM)
	mango-run $<

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

LD_FLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld -lmango -lmango_gcc
%.elf: %.o $(LIB_SOURCES)
	riscv64-unknown-elf-ld $^ $(LD_FLAGS) -o $@ 

ARCH = -march=rv64im_zicsr -mabi=lp64 
CFLAGS = $(ARCH) -g -Og -I$$CS107E/include $$warn $$freestanding -fno-omit-frame-pointer -fstack-protector-strong -Wno-builtin-declaration-mismatch #no-builtin-decoration-mismatch because we implement math functions with different type signatures
%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@


clean: #-f for it to shut up
	rm -f *.o *.bin *.elf


tests/maths: tests/maths.bin
	mango-run $<

