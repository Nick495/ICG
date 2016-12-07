#include "icg.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

ubig icg_period(struct icg_tuple t)
{
	ubig *seq = icg(t);
	ubig len = seq_len(t.mod, seq);
	free(seq);
	return len;
}

big rand_odd(big max)
{
	big x;
	x = rand() % max;
	if (x % 2) {
		x += 1;
	}
	return x;
}

void find(big modu)
{
	time_t tm;
	srand((unsigned) time(&tm));

	struct icg_tuple t;
	t.mod = modu;
	for (int i = 0; i < 1000; i++) {
		t.mul = rand_odd(modu);
		t.add = rand_odd(modu);
		t.seed = rand_odd(modu);
		printf("%lld\t%lld\t%lld\t%lld\t%llu\n",
			t.mod, t.mul, t.add, t.seed, icg_period(t));
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <modulus>\n", argv[0]);
		return 1;
	}
	find(strtoull(argv[1], NULL, 10));
	return 0;
}
