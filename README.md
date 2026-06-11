# pi-mos

A minimal (bare-metal) operating system for the Raspberry Pi 3B+

## Emulation

While this works best on a physical Raspberry Pi, this OS works under emulation. Running the command

```qemu-system-aarch64 -machine raspi3b -serial null -serial mon:stdio -kernel ./kernel8.elf```

after calling `make` should emulate the OS. 

## Resources

This project was inspired by the following bare-metal OS implementations in the PI 4 and 3B respectively:

- https://www.rpi4os.com/
- https://github.com/s-matyukevich/raspberry-pi-os

The Pi 3B/3B+ uses a Broadcom BCM2837 processor. An [unofficial datasheet](https://github.com/raspberrypi/documentation/issues/325#issuecomment-379651504) was used for this project.

Communication with the VideoCore GPU is detailed [here](https://github.com/raspberrypi/firmware/wiki).
