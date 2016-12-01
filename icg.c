#include <stdio.h>	/* printf() */
#include <stdlib.h>	/* strtoull() */
#include <string.h>	/* memset */
#include <assert.h>	/* assert() */

typedef long long big;
typedef unsigned long long ubig;

/* TODO: Fix mod_inv such that ts can't be negative.
 * Taken from wikipedia's page. Ref:
 * https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Modular_integers
*/
static ubig mod_inv(big value, big modulus)
{
	big t = 0, newt = 1;
	big r = modulus, newr = value;
	while (newr != 0) {
		big quotient = r / newr;
		/* (t, newt) := (newt, t - quotient * newt) */
		big swap = newt;
		newt = t - quotient * newt;
		t = swap;

		/* (r, newr) := (newr, r - quotient * newr)*/
		swap = newr;
		newr = r - quotient * newr;
		r = swap;
	}
	if (r > 1) { /* value is not invertible. */
		return 0;
	}
	if (t < 0) { /* TODO: Prevent this case. */
		t += modulus;
	}
	return t;
}

static void gen_invs(big modulus, ubig invs[modulus])
{
	for (size_t i = 0; i < modulus; ++i) {
		invs[i] = mod_inv(i, modulus);
	}
}

static void gen_seq(big modulus, big mul, big add, big seed,
		ubig invs[modulus], ubig arr[modulus])
{
	big value = seed;
	for (size_t i = 0; i < modulus; ++i) {
		arr[i] = value;
		if (invs[value]) {
			value = (mul * invs[value] + add) % modulus;
		} else {
			value = add;
		}
	}
}

static ubig seq_len(ubig size, ubig seq[size])
{
	char *seen = malloc(sizeof(char));
	memset(&seen, 0, sizeof(char) * size);

	for (ubig i = 0; i < size; ++i) {
		if (seen[seq[i]]) {
			return i + 1;
		}
		seen[seq[i]] = 1;
	}
	free(seen);
	return size;
}

static void print_arr(big size, ubig arr[size])
{
	for (size_t i = 0; i < size; ++i) {
		printf("%llu ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	if (argc != 5) {
		printf("usage: %s <modulus> <multiplier> <adder> <seed>\n",
				argv[0]);
		return 1;
	}
	big modulus = strtoull(argv[1], NULL, 10);
	big multiplier = strtoull(argv[2], NULL, 10) % modulus;
	big adder = strtoull(argv[3], NULL, 10) % modulus;
	big seed = strtoull(argv[4], NULL, 10) % modulus;
	assert(modulus >= 0 && multiplier >= 0 && seed >= 0);
	assert(multiplier < modulus && adder < modulus && seed < modulus);

	ubig *invs = malloc(sizeof(ubig) * modulus);
	ubig *seq = malloc(sizeof(ubig) * modulus);
	gen_invs(modulus, invs);
	gen_seq(modulus, multiplier, adder, seed, invs, seq);

	printf("DEBUG: Inverses:\n");
	print_arr(modulus, invs);

	printf("DEBUG: Sequence:\n");
	print_arr(modulus, seq);

	printf("Sequence length: %llu\n", seq_len(modulus, seq));

	free(invs);
	free(seq);
}
