#include <stdint.h>

#define XY(x, y) ((x) + (y) * 160)

#define REG_DISPCNT *((volatile uint16_t *) 0x04000000)

#define VRAM_M5_FRONT ((uint16_t *) 0x06000000)
#define VRAM_M5_BACK  ((uint16_t *) 0x0600A000)

// TODO buffer swapping

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

int main(int argc, char *argv[]) {

    // https://www.problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
    
    // set background mode to mode 5 and display BG2
    REG_DISPCNT = (5 & 0x7) | (1 << 10);

    while (1) {

        // recognizes x keypress
        uint16_t keyinput = *((volatile uint16_t *) 0x04000130) & 0b0000000000000001;

        VRAM_M5_BACK[XY(120 + keyinput, 80)] = RGB15(31, 0, 0);
        VRAM_M5_BACK[XY(136 + keyinput, 80)] = RGB15(0, 31, 0);
        VRAM_M5_BACK[XY(120 + keyinput, 64)] = RGB15(0, 0, 31);

        // flip bit in display control register responsible for selecting which buffer is front
        REG_DISPCNT = REG_DISPCNT ^ (1 << 4);
    }

    return 0;
}
