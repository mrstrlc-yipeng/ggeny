#include <stdio.h>
#include <stdint.h>

uint32_t x, y, z, w;

uint32_t xorshift128(void) {
    uint32_t t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

int main(void) {
    x = 128;
    int i;
    int counter[2];
    for (i = 0; i < 2; i++) {
        counter[i] = 0;
    }
    for (i = 0; i < 100; i++) {
        counter[xorshift128() % 2]++;
    }
    for (i = 0; i < 2; i++) {
        printf("times of [%d] = %d\n", i, counter[i]);
    }
}
