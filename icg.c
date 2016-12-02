#include "icg.h"

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

		big swap = newt; /* (t, newt) := (newt, t - quotient * newt) */
		newt = t - quotient * newt;
		t = swap;

		swap = newr; /* (r, newr) := (newr, r - quotient * newr)*/
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

static void gen_invs(big modulus, ubig *invs)
{
	for (size_t i = 0; i < (size_t)modulus; ++i) {
		invs[i] = mod_inv(i, modulus);
	}
}

static void gen_seq(big mod, big mul, big add, big seed, ubig *invs, ubig *arr)
{
	big value = seed;
	for (size_t i = 0; i < (size_t)mod; ++i) {
		arr[i] = value;
		if (invs[value]) {
			value = (mul * invs[value] + add) % mod;
		} else {
			value = add;
		}
	}
}

/* Debugging */
static void print_arr(big size, ubig arr[size])
{
	for (size_t i = 0; i < (size_t)size; ++i) {
		printf("%llu ", arr[i]);
	}
	printf("\n");
}

ubig *icg(big modulus, big mul, big add, big seed)
{
	ubig *invs = malloc(sizeof(ubig) * modulus);
	ubig *seq = malloc(sizeof(ubig) * modulus);

	assert(mul >= 0 && add >= 0 && seed >= 0);
	mul %= modulus;
	add %= modulus;
	seed %= modulus;

	gen_invs(modulus, invs);
	gen_seq(modulus, mul, add, seed, invs, seq);
	free(invs);
	return seq;
}
