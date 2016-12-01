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
	char *seen = malloc(sizeof(char) * size);
	memset(seen, 0, sizeof(char) * size);
	for (ubig i = 0; i < size; ++i) {
		//printf("%lld\n", seq[i]);
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

