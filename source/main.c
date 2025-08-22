#include <stdint.h>
#include <string.h>

#define REG16(reg)   *((volatile uint16_t *) (reg))

// display
// relevant sections:
// - LCD VRAM Overview
// - LCD I/O BG Control
// - DISPCNT  https://www.problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
// - Palettes https://www.problemkaputt.de/gbatek.htm#lcdcolorpalettes
#define DISPCNT      REG16(0x04000000)
#define BG0CNT       REG16(0x04000008)
#define VRAM         REG16(0x06000000)
#define PALETTE      REG16(0x05000000)

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

    memset((void *) &PALETTE, RGB15(31, 31, 31), 256);
    
    // set background mode to mode 0 and display BG 0 (not 123 for now)
    DISPCNT = 0 | (0b0001 << 8);

    // configure BG0CNT
    // BG0CNT = 

    while (1) {

    }

    return 0;
}
