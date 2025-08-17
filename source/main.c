#include <stdint.h>

#define XY(x, y) ((x) + (y) * 240)

#define REG_DISPCNT             *((volatile uint16_t *) 0x04000000)

#define DISPCNT_BG_MODE(n)      ((n) & 0x7) // 0 to 5

#define DISPCNT_BG2_ENABLE      (1 << 10)

#define MEM_VRAM_MODE3_FB       ((uint16_t *) 0x06000000)

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

int main(int argc, char *argv[]) {
    
    REG_DISPCNT = DISPCNT_BG_MODE(3) | DISPCNT_BG2_ENABLE;

    while (1) {

        // recognizes x keypress
        uint16_t keyinput = *((volatile uint16_t *) 0x04000130) & 0b0000000000000001;

        MEM_VRAM_MODE3_FB[XY(120 + keyinput, 80)] = RGB15(31, 0, 0);
        MEM_VRAM_MODE3_FB[XY(136 + keyinput, 80)] = RGB15(0, 31, 0);
        MEM_VRAM_MODE3_FB[XY(120 + keyinput, 96)] = RGB15(0, 0, 31);
    }

    return 0;
}
