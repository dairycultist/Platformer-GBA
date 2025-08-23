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
#define PALETTE(index)           REG16(0x05000000 + 0x2 * index)                    // https://www.problemkaputt.de/gbatek.htm#lcdcolorpalettes
#define VRAM_TILE(sector, index) REG16(0x06000000 + 0x4000 * sector + 0x20 * index) // https://www.problemkaputt.de/gbatek.htm#lcdvramoverview
#define VRAM_MAP(sector, index)  REG16(0x06000000 + 0x800 * sector + 0x2 * index)

// input
#define KEYINPUT     REG16(0x04000130)
#define BUTTON_A     !(REG_KEYINPUT & (1))
#define BUTTON_B     !(REG_KEYINPUT & (1 << 1))
#define BUTTON_RIGHT !(REG_KEYINPUT & (1 << 4))
#define BUTTON_LEFT  !(REG_KEYINPUT & (1 << 5))
#define BUTTON_UP    !(REG_KEYINPUT & (1 << 6))
#define BUTTON_DOWN  !(REG_KEYINPUT & (1 << 7))

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

int main(int argc, char *argv[]) {

    // initialize rendering
    PALETTE(0) = RGB15(0, 0, 0);                        // initialize basic palette
    PALETTE(1) = RGB15(31, 31, 31);
    PALETTE(2) = RGB15(31, 10, 31);
    DISPCNT = 0 | (0b0001 << 8);                        // set background mode to mode 0 and display BG 0 (not 123 for now)
    BG0CNT = (8 << 8);                                  // configure BG0CNT to take tile data from 0th sector, and map data from 8th sector

    // write a test tile to tile data
    memcpy((void *) (&VRAM_TILE(0, 1)), test_tile, 0x20);

    // write a test map to map data
    VRAM_MAP(8, 0) = 1;
    VRAM_MAP(8, 1) = 1;

    // TODO use input to scroll the background

    // TODO render an object

    while (1);

    return 0;
}
