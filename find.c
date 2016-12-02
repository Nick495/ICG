#include "icg.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static ubig seq_len(ubig size, ubig seq[size])
{
	char *seen = malloc(sizeof(char) * size);
	memset(seen, 0, sizeof(char) * size);
	for (size_t i = 0; i < (size_t) size; ++i) {
		if (seen[seq[i]]) {
			return i + 1;
		}
		seen[seq[i]] = 1;
	}
	free(seen);
	return size;
}

ubig icg_period(big modulus, big mul, big add, big seed)
{
	ubig *seq = icg(modulus, mul, add, seed);
	ubig len = seq_len(modulus, seq);
	free(seq);
	return len;
}

big rand_odd(big modu)
{
    big x;
    do  {
	x = rand();
    } while (x > modu || x < 0 || x % 2);
    return x;
}

void find(big modu)
{
#if 0
	FILE* fp;
	fp = fopen("search.txt", 'w');
	fprintf(fp, "");
	fclose(fp);
#endif

	time_t t;
	srand((unsigned) time(&t));

	big mult, addr, seed;
	for (int i = 0; i < 1000; i++) {
		mult = rand_odd(modu);
		addr = rand_odd(modu);
		seed = rand_odd(modu);
		printf("%lld\t%lld\t%lld\t%lld\t%llu\n", modu, mult, addr, seed,
				icg_period(modu, mult, addr, seed));
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <modulus>\n", argv[0]);
	}
	find(strtoull(argv[1], NULL, 10));
	return 0;
}
