# Karting GBA

[minicraft plus](https://minicraftplus.github.io/) is cool because it has its own dedicated website (with info + download), a small community, and a small piece of software that gets updates. I wanna do that

I can design a little [game cartridge](https://en.wikipedia.org/wiki/Game_Boy_Game_Pak)!

if developing 3D on hardware not designed for it proves too challenging a task, ill just pivot to a cute platformer á la kirby

a 3D karting game for GBA. a fun project because I actually like karting games, so im not JUST making it for the process but also the result.

has some fun technical challenges because GBA is Not particularly built for 3D, and also just figuring out how GBA development works in general. fixed-point math and sine lookup tables (indexed 0-511)!! since floats on GBA are SO SLOW

referencing this project that WORKS YAY (after doing `brew install gcc-arm-none-eabi`) im working on cutting out all the parts that I dont want cuz liteweight https://github.com/AntonioND/gba-bootstrap

GBA specs, including memory register locations https://www.problemkaputt.de/gbatek.htm

emulator https://mgba.io/

example of such game https://github.com/Vulcalien/minicraft-gba

render modes (aka background modes) 0-5 https://www.patater.com/gbaguy/gba/ch5.htm

There are libraries like `libgba` and `libtonc` for developing for GBA, neither of which I will be using.
