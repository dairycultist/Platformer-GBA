#include <stdint.h>

#define XY(x, y) ((x) + (y) * 160)

// https://www.problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
#define REG_DISPCNT *((volatile uint16_t *) 0x04000000)
#define REG_KEYINPUT *((volatile uint16_t *) 0x04000130)

#define VRAM_M5_BUFA ((uint16_t *) 0x06000000)
#define VRAM_M5_BUFB ((uint16_t *) 0x0600A000)

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

static inline void draw_mode7(uint16_t *const buffer, int x, int y) {

    for (int screen_y = 16; screen_y < 128; screen_y++) {

        int sample_y = 4096 / screen_y + y;

        for (int screen_x = 0; screen_x < 160; screen_x++) {

            int sample_x = (screen_x - 80) * (1 + 2048 / screen_y) / 64 + x;

            if (sample_x >= 0 && sample_y >= 0) {
                buffer[XY(screen_x, screen_y)] = RGB15(((sample_x / 16 % 2) ^ (sample_y / 16 % 2)) * 31, 0, 0);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    
    // set background mode to mode 5 and display BG2
    REG_DISPCNT = 5 | (1 << 10);

    int x = 0, y = 0;

    while (1) {

        uint16_t action_x = !(REG_KEYINPUT & (1));
        uint16_t action_z = !(REG_KEYINPUT & (1 << 1));

        if (action_x) {
            x++;
        }
        if (action_z) {
            y++;
        }

        // render to back buffer
        uint16_t *back_buffer = REG_DISPCNT & (1 << 4) ? VRAM_M5_BUFA : VRAM_M5_BUFB;

        draw_mode7(back_buffer, x, y);

        // flip bit in display control register responsible for selecting which buffer is front
        REG_DISPCNT = REG_DISPCNT ^ (1 << 4);
    }

    return 0;
}
