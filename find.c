#include "icg.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

ubig icg(int modulus, int multiplier, int adder, int seed) {
    multiplier %= modulus;
    adder %= modulus;
    seed %= modulus;
    if(modulus < 0 || multiplier < 0 || seed < 0) return -1;
    ubig *invs = malloc(sizeof(ubig) * modulus);
	ubig *seq = malloc(sizeof(ubig) * modulus);
	gen_invs(modulus, invs);
	gen_seq(modulus, multiplier, adder, seed, invs, seq);
    ubig period = seq_len(modulus, seq); 
    free(invs);
	free(seq);
    return period;
}

int rand_odd(int modu) {
    int x = rand() % modu;
    if (x % 2 == 0) 
        x++;
    return x;
}

void find() {
    //FILE* fp;
    //fp = fopen("search.txt", 'w');
    //fprintf(fp, "");
    //fclose(fp);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int modu, mult, addr, seed;
    ubig period;
    modu = 4096;
    for (int i = 0; i < 1000; i++) {
        mult = rand_odd(modu);
        addr = rand_odd(modu);
        seed = rand_odd(modu);
        period = icg(modu, mult, addr, seed);
        printf("%d\t%d\t%d\t%d\t%llu\n", modu, mult, addr, seed, period);
    }   
}


int main() {
    find();
    return 0;
}
