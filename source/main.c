#include <stdint.h>

#define XY(x, y) ((x) + (y) * 160)

#define REG_DISPCNT *((volatile uint16_t *) 0x04000000)

#define VRAM_M5_BUFA ((uint16_t *) 0x06000000)
#define VRAM_M5_BUFB ((uint16_t *) 0x0600A000)

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

int main(int argc, char *argv[]) {

    // https://www.problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
    
    // set background mode to mode 5 and display BG2
    REG_DISPCNT = 5 | (1 << 10);

    int tick = 0;

    while (1) {

        tick++;

        // uint16_t action_x = *((volatile uint16_t *) 0x04000130) & 0b0000000000000001;
        // uint16_t action_z = *((volatile uint16_t *) 0x04000130) & 0b0000000000000010;

        // render to back buffer
        uint16_t *back_buffer = REG_DISPCNT & (1 << 4) ? VRAM_M5_BUFA : VRAM_M5_BUFB;

        for (int y=0; y<128; y++) {

            int sample_y = (y >> 2) + tick;

            for (int x=0; x<160; x++) {

                int sample_x = x / 4;

                back_buffer[XY(x, y)] = RGB15(sample_x % 32, sample_y % 32, 0);
            }
        }

        // flip bit in display control register responsible for selecting which buffer is front
        REG_DISPCNT = REG_DISPCNT ^ (1 << 4);
    }

    return 0;
}
