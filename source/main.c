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
//   array += Math.floor(Math.sin((i * Math.PI) / 128) * 128) + ',';
// console.log(array);
static int8_t sin128_lookup[256] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57,59,62,65,67,70,73,75,78,80,82,85,87,89,91,94,96,98,100,102,103,105,107,108,110,112,113,114,116,117,118,119,120,121,122,123,123,124,125,125,126,126,126,126,126,127,126,126,126,126,126,125,125,124,123,123,122,121,120,119,118,117,116,114,113,112,110,108,107,105,103,102,100,98,96,94,91,89,87,85,82,80,78,75,73,70,67,65,62,59,57,54,51,48,45,42,39,36,33,30,27,24,21,18,15,12,9,6,3,0,-4,-7,-10,-13,-16,-19,-22,-25,-28,-31,-34,-37,-40,-43,-46,-49,-52,-55,-58,-60,-63,-66,-68,-71,-74,-76,-79,-81,-83,-86,-88,-90,-92,-95,-97,-99,-101,-103,-104,-106,-108,-109,-111,-113,-114,-115,-117,-118,-119,-120,-121,-122,-123,-124,-124,-125,-126,-126,-127,-127,-127,-127,-127,-127,-127,-127,-127,-127,-127,-126,-126,-125,-124,-124,-123,-122,-121,-120,-119,-118,-117,-115,-114,-113,-111,-109,-108,-106,-104,-103,-101,-99,-97,-95,-92,-90,-88,-86,-83,-81,-79,-76,-74,-71,-68,-66,-63,-60,-58,-55,-52,-49,-46,-43,-40,-37,-34,-31,-28,-25,-22,-19,-16,-13,-10,-7,-4};

#define SIN(a) (sin128_lookup[a % 256])
#define COS(a) (sin128_lookup[(a + 64) % 256])

static inline uint16_t RGB15(uint16_t r, uint16_t g, uint16_t b) {

    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

static inline void draw_mode7(uint16_t *const buffer, int x, int y, int a) {

    for (int screen_y = 16; screen_y < 128; screen_y++) {

        int persp_y = 8192 / screen_y;

        for (int screen_x = 0; screen_x < 160; screen_x++) {

            int persp_x = (screen_x - 80) * (1 + 8192 / screen_y) / 128;

            int sample_x = persp_x * COS(a) / 128 - persp_y * SIN(a) / 128 + x;
            int sample_y = persp_x * SIN(a) / 128 + persp_y * COS(a) / 128 + y;

            if (sample_x >= 0 && sample_y >= 0) {
                buffer[XY(screen_x, screen_y)] = RGB15(((sample_x / 16 % 2) ^ (sample_y / 16 % 2)) * 31, 0, 0);
            } else {
                buffer[XY(screen_x, screen_y)] = 0;
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

        draw_mode7(back_buffer, y, y, -x);

        // flip bit in display control register responsible for selecting which buffer is front
        REG_DISPCNT = REG_DISPCNT ^ (1 << 4);
    }

    return 0;
}
