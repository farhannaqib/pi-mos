CFILES = $(wildcard src/*.c)
SFILES = $(wildcard src/*.S)
COFILES = $(CFILES:.c=.co)
SOFILES = $(SFILES:.S=.so)
LLVMPATH = /opt/homebrew/opt/llvm/bin
LDPATH = /opt/homebrew/bin
CLANGFLAGS = --target=aarch64-elf -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd -Iinclude 

all: clean kernel8.img

%.so: %.S 
	$(LLVMPATH)/clang $(CLANGFLAGS) -c $< -o $@

%.co: %.c
	$(LLVMPATH)/clang $(CLANGFLAGS) -c $< -o $@

kernel8.img: $(COFILES) $(SOFILES)
	$(LDPATH)/ld.lld -m aarch64elf -nostdlib $(COFILES) $(SOFILES) -T link.ld -o kernel8.elf
	$(LLVMPATH)/llvm-objcopy -O binary kernel8.elf kernel8.img

clean:
	/bin/rm kernel8.elf src/*.o src/*.co src/*.so *.img > /dev/null 2> /dev/null || true
