// rand.c
unsigned int rand_seed = 0xDEADBEEF;  // Starting seed

unsigned int rand(void) {
    rand_seed = rand_seed * 1664525 + 1013904223; // Linear congruential generator
    return rand_seed;
}
