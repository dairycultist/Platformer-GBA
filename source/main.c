#include <stdint.h>

#define XY(x, y) ((x) + (y) * 160)

// https://www.problemkaputt.de/gbatek.htm#lcdiodisplaycontrol
#define REG_DISPCNT *((volatile uint16_t *) 0x04000000)
#define REG_KEYINPUT *((volatile uint16_t *) 0x04000130)

#define VRAM_M5_BUFA ((uint16_t *) 0x06000000)
#define VRAM_M5_BUFB ((uint16_t *) 0x0600A000)

// // JS code to precompute
// let array = '';
// for (let i = 0; i < 256; i++)
//   array += Math.floor(Math.sin((i * Math.PI) / 128) * 100) + ',';
// console.log(array);
static int sin100_lookup[256] = {0,2,4,7,9,12,14,17,19,21,24,26,29,31,33,35,38,40,42,44,47,49,51,53,55,57,59,61,63,65,67,68,70,72,74,75,77,78,80,81,83,84,85,87,88,89,90,91,92,93,94,94,95,96,97,97,98,98,98,99,99,99,99,99,100,99,99,99,99,99,98,98,98,97,97,96,95,94,94,93,92,91,90,89,88,87,85,84,83,81,80,78,77,75,74,72,70,68,67,65,63,61,59,57,55,53,51,49,47,44,42,40,38,35,33,31,29,26,24,21,19,17,14,12,9,7,4,2,0,-3,-5,-8,-10,-13,-15,-18,-20,-22,-25,-27,-30,-32,-34,-36,-39,-41,-43,-45,-48,-50,-52,-54,-56,-58,-60,-62,-64,-66,-68,-69,-71,-73,-75,-76,-78,-79,-81,-82,-84,-85,-86,-88,-89,-90,-91,-92,-93,-94,-95,-95,-96,-97,-98,-98,-99,-99,-99,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-99,-99,-99,-98,-98,-97,-96,-95,-95,-94,-93,-92,-91,-90,-89,-88,-86,-85,-84,-82,-81,-79,-78,-76,-75,-73,-71,-69,-68,-66,-64,-62,-60,-58,-56,-54,-52,-50,-48,-45,-43,-41,-39,-36,-34,-32,-30,-27,-25,-22,-20,-18,-15,-13,-10,-8,-5,-3};

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

static inline void draw_mode7(uint16_t *const buffer, int x, int y) {

    for (int screen_y = 16; screen_y < 128; screen_y++) {

        int sample_y = 4096 / screen_y + y;

        for (int screen_x = 0; screen_x < 160; screen_x++) {

            int sample_x = (screen_x - 80) * (1 + 8192 / screen_y) / 256 + x;

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

        for (int i = 0; i < 256; i++) {
            back_buffer[XY(30 + sin100_lookup[i] / 4, 30 + sin100_lookup[(i + 64) % 256] / 4)] = RGB15(31, 31, 31);
        }

        // flip bit in display control register responsible for selecting which buffer is front
        REG_DISPCNT = REG_DISPCNT ^ (1 << 4);
    }

    return 0;
}
