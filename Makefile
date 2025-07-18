CFILES = $(wildcard src/*.c)
OFILES = $(CFILES:.c=.o)
LLVMPATH = /opt/homebrew/opt/llvm/bin
LDPATH = /opt/homebrew/bin
CLANGFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd -Iinclude

all: clean kernel8.img

src/boot.o: src/boot.S
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c src/boot.S -o src/boot.o

%.o: %.c
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c $< -o $@

kernel8.img: src/boot.o $(OFILES)
	$(LDPATH)/ld.lld -m aarch64elf -nostdlib src/boot.o $(OFILES) -T link.ld -o kernel8.elf
	$(LLVMPATH)/llvm-objcopy -O binary kernel8.elf kernel8.img

clean:
	/bin/rm kernel8.elf src/*.o *.img > /dev/null 2> /dev/null || true
