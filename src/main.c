#include <stdint.h>
#include <string.h>
#include "res.c"

#define REG16(reg)   *((volatile uint16_t *) (reg))

// display
#define DISPCNT                  REG16(0x04000000)                                  // https://www.problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
#define BG0CNT                   REG16(0x04000008)                                  // https://www.problemkaputt.de/gbatek.htm#lcdiobgcontrol
#define BG1CNT                   REG16(0x0400000A)
#define BG2CNT                   REG16(0x0400000C)
#define BG3CNT                   REG16(0x0400000E)
#define BG0HOFS                  REG16(0x04000010)                                  // https://www.problemkaputt.de/gbatek.htm#lcdiobgscrolling
#define BG0VOFS                  REG16(0x04000012)
#define PALETTE(index)           REG16(0x05000000 + 0x2 * index)                    // https://www.problemkaputt.de/gbatek.htm#lcdcolorpalettes
#define VRAM_TILE(sector, index) REG16(0x06000000 + 0x4000 * sector + 0x20 * index) // https://www.problemkaputt.de/gbatek.htm#lcdvramoverview
#define VRAM_MAP(sector, index)  REG16(0x06000000 + 0x800 * sector + 0x2 * index)

// input
#define KEYINPUT     REG16(0x04000130)
#define BUTTON_A     !(KEYINPUT & (1))
#define BUTTON_B     !(KEYINPUT & (1 << 1))
#define BUTTON_RIGHT !(KEYINPUT & (1 << 4))
#define BUTTON_LEFT  !(KEYINPUT & (1 << 5))
#define BUTTON_UP    !(KEYINPUT & (1 << 6))
#define BUTTON_DOWN  !(KEYINPUT & (1 << 7))

// interrupts https://www.problemkaputt.de/gbatek.htm#gbainterruptcontrol
#define IME REG16(0x04000208)
#define IE  REG16(0x04000200)
#define IF  REG16(0x04000202)

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

// TODO render an object

int main(int argc, char *argv[]) {

    // initialize rendering
    PALETTE(0) = RGB15(0, 0, 0);    // initialize basic palette
    PALETTE(1) = RGB15(31, 31, 31);
    PALETTE(2) = RGB15(31, 10, 31);
    DISPCNT = 0 | (0b0001 << 8);    // set background mode to mode 0 and display BG 0 (not 123 for now)
    BG0CNT = (8 << 8);              // configure BG0CNT to take tile data from 0th sector, and map data from 8th sector

    // enable VBlank interrupt
    IME = 1;
    IE = 1;

    // write a test tile to tile data
    memcpy((void *) (&VRAM_TILE(0, 1)), test_tile, 0x20);

    // write a test map to map data
    VRAM_MAP(8, 0) = 1;
    VRAM_MAP(8, 1) = 1;

    while (1) {
        // TODO use input to scroll the background
        if (BUTTON_RIGHT)
            BG0VOFS = 1;

        // wait on VBlank interrupt
        IF = 1;
        while (IF);
    }

    return 0;
}
