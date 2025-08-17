# Karting GBA

[minicraft plus](https://minicraftplus.github.io/) is cool because it has its own dedicated website (with info + download), a small community, and a small piece of software that gets updates. I wanna do that

what if I made a 3D karting game for GBA

I think that would be a fun project

especially because I actually like karting games, so im not JUST making it for the process but also the result

has some fun technical challenges because GBA is Not particularly built for 3D, and also just figuring out how GBA development works in general

```
arm-none-eabi-gcc -o rom.elf <C files>
arm-none-eabi-objcopy -O binary rom.elf rom.gba
```

startup project that WORKS YAY (after doing `brew install gcc-arm-none-eabi`) https://github.com/AntonioND/gba-bootstrap

GBA specs, including memory register locations https://www.problemkaputt.de/gbatek.htm

emulator https://mgba.io/

example of such game https://github.com/Vulcalien/minicraft-gba

There are libraries like `libgba` and `libtonc` for developing for GBA, neither of which I will be using.
