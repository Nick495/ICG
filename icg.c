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

static void gen_seq(struct icg_tuple t, ubig *invs, ubig *seq)
{
	big value = t.seed;
	for (size_t i = 0; i < (size_t)t.mod; ++i) {
		seq[i] = value;
		if (invs[value]) {
			value = (t.mul * invs[value] + t.add) % t.mod;
		} else {
			value = t.add;
		}
	}
}

/* Assumes all icgs use the same modulus. */
static void gen_cicg_seq(struct icg_tuple *av, size_t avlen, ubig *inv, ubig *s)
{
	assert(av != NULL);
	big v = av[0].seed;
	for (size_t i = 0; i < (size_t)av[0].mod; ++i) {
		for (size_t j = 0; j < avlen; ++j) {
			s[i * avlen + j] = v;
			if (inv[v]) {
				v = (av[j].mul* inv[v] + av[j].add) % av[j].mod;
			} else {
				v = av[j].add;
			}
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

ubig *icg(struct icg_tuple args)
{
	ubig *invs = malloc(sizeof(ubig) * args.mod);
	ubig *seq = malloc(sizeof(ubig) * args.mod);

	assert(args.mul >= 0 && args.add >= 0 && args.seed >= 0);
	args.mul %= args.mod;
	args.add %= args.mod;
	args.seed %= args.mod;

	gen_invs(args.mod, invs);
	gen_seq(args, invs, seq);
	free(invs);
	return seq;
}

/* Assumes all icgs use the same modulus. */
ubig *cicg(struct icg_tuple *argvec, size_t avcnt)
{
	assert(argvec != NULL);
	ubig *invs = malloc(sizeof(*invs) * argvec[0].mod);
	ubig *seq = malloc(sizeof(*seq) * argvec[0].mod * avcnt);

	for (size_t i = 0; i < avcnt; ++i) {
		argvec[i].mul %= argvec[i].mod;
		argvec[i].add %= argvec[i].mod;
		argvec[i].seed %= argvec[i].mod;

		struct icg_tuple a = argvec[i];
		assert(a.mul >= 0 && a.add >= 0 && a.seed >= 0);
	}

	gen_invs(argvec[0].mod, invs);
	gen_cicg_seq(argvec, avcnt, invs, seq);
	free(invs);
	return seq;
}
