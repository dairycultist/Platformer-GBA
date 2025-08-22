# Platformer GBA

cute platformer for GBA á la kirby. finding out how GBA development/hardware works is fun, and playing my game will also be fun (eventually)

GBA specs, including memory register locations https://www.problemkaputt.de/gbatek.htm

render modes (aka background modes) 0-5 https://www.patater.com/gbaguy/gba/ch5.htm

emulator https://mgba.io/

I can design a little [game cartridge](https://en.wikipedia.org/wiki/Game_Boy_Game_Pak)!

lookup tables and fixed-point math

```
format 000000000000(.)0000
    range of 4096
    16 decimal values
    no sign

addition/subtraction is the same
multiplication: (a * b) >> (decimal places * 2)
```

referencing [this project](https://github.com/AntonioND/gba-bootstrap) that WORKS YAY (after doing `brew install gcc-arm-none-eabi`).

[minicraft plus](https://minicraftplus.github.io/) is cool because it has its own dedicated website (with info + download), a small community, and a small piece of software that gets updates. I wanna do that

There are libraries like `libgba` and `libtonc` for developing for GBA, neither of which I will be using because BLOAT (and I like doing the fun stuff myself).
