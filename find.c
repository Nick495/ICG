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
	do  {
		x = rand();
	} while (x > max || x < 0 || x % 2);
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
	}
	find(strtoull(argv[1], NULL, 10));
	return 0;
}
