CFILES := $(shell find src -name '*.c')
SFILES := $(shell find src -name '*.S')
COFILES = $(CFILES:.c=.co)
SOFILES = $(SFILES:.S=.so)
LLVMPATH = /opt/homebrew/opt/llvm/bin
LDPATH = /opt/homebrew/bin
CLANGFLAGS = --target=aarch64-elf -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd -Iinclude \
	-Iinclude/drivers \
	-Iinclude/hw \
	-Iinclude/kernel \
	-Iinclude/lib 

all: clean kernel8.img

%.so: %.S 
	$(LLVMPATH)/clang $(CLANGFLAGS) -c $< -o $@

%.co: %.c
	$(LLVMPATH)/clang $(CLANGFLAGS) -c $< -o $@

kernel8.img: $(COFILES) $(SOFILES)
	$(LDPATH)/ld.lld -m aarch64elf -nostdlib $(COFILES) $(SOFILES) -T link.ld -o kernel8.elf
	$(LLVMPATH)/llvm-objcopy -O binary kernel8.elf kernel8.img

clean:
	find src -name '*.co' -delete
	find src -name '*.so' -delete
	/bin/rm kernel8.elf *.img > /dev/null 2> /dev/null || true
