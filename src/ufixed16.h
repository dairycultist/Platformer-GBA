// fixed-point math

// format 000000000(.)0000000
//     range of 512 (same as BG offset)
//     128 decimal values
//     no sign
typedef uint16_t ufixed16;

#define FIXED_TO_INT(fixed) ((fixed) >> 7)
#define INT_TO_FIXED(int)   ((int) << 7)

#define FIXED_MULT(a, b)    ((uint16_t) ((((uint32_t) (a)) * ((uint32_t) (b))) >> 14))

// addition/subtraction is the same
// division?