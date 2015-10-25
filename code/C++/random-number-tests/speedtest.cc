#define SIMPLE_SPRNG

#include "sprng_cpp.h"
#include "fasttime.h"

#include <stdio.h>

int main (int argc __attribute__((unused)), char *argv[]  __attribute__((unused))) {
    const int N = 100000000;
    {
        fasttime_t start = gettime();
        int sum = 0;
        for (int i = 0; i < N; i++) {
            sum += random();
        }
        fasttime_t end   = gettime();
        printf("random: %fs (sum=%d)\n", end-start, sum);
    }
    {
        fasttime_t start = gettime();
        int sum = 0;
        for (int i = 0; i < N; i++) {
            sum += isprng();
        }
        fasttime_t end   = gettime();
        printf("isprng: %fs (sum=%d)\n", end-start, sum);
    }
    return 0;
}
