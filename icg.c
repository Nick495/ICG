#include "icg.h"

#if TEST
#include <stdio.h>
static void print_arr(big size, ubig arr[size]);
#endif


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
	for (size_t i = 0; i < (size_t)t.mod + 1; ++i) {
		seq[i] = value;
		if (invs[value]) {
			value = (t.mul * invs[value] + t.add) % t.mod;
		} else {
			value = t.add;
		}
	}
}

ubig *icg(struct icg_tuple args)
{
	ubig *invs = malloc(sizeof(ubig) * args.mod);
	ubig *seq = malloc(sizeof(ubig) * (args.mod + 1));

	assert(args.mul >= 0 && args.add >= 0 && args.seed >= 0);
	args.mul %= args.mod;
	args.add %= args.mod;
	args.seed %= args.mod;

	gen_invs(args.mod, invs);
	gen_seq(args, invs, seq);
	free(invs);
	return seq;
}

static void binary_out(ubig val)
{
	write(STDOUT_FILENO, &val, sizeof(val) / 4);
}

static void ascii_out(ubig val)
{
	printf("%llu\n", val);
}

/* Assumes all icgs use the same modulus. */
ubig *cicg(struct icg_tuple *params, size_t cnt)
{
	ubig *seqs[cnt]; /* Inputs */
	ubig prod = 1; /* Initial value of 1 */
	for (size_t i = 0; i < cnt; ++i) {
		prod *= params[i].mod;
		seqs[i] = icg(params[i]);
	}
	ubig *seq = calloc(prod, sizeof(*seq)); /* Output sequence. */

	for (size_t i = 0; i < prod; ++i) {
		ubig val = 0;
		for (size_t j = 0; j < cnt; ++j) {
			const ubig mod = params[j].mod;
			val += (prod / mod) * seqs[j][i % mod];
		}
		seq[i] = val % prod;
	}

	for (size_t i = 0; i < cnt; ++i) {
		free(seqs[i]);
	}
	return seq;
}

/* Assumes all icgs use the same modulus. */
void cicg_writer(struct icg_tuple *params, size_t cnt, int binary_mode)
{
	ubig *seqs[cnt]; /* Inputs */
	ubig prod = 1; /* Initial value of 1 */
	void (*out)(ubig); /* Output function */
	if (binary_mode) {
		out = binary_out;
	} else {
		out = ascii_out;
	}
	for (size_t i = 0; i < cnt; ++i) {
		prod *= params[i].mod;
		seqs[i] = icg(params[i]);
	}

	for (size_t i = 0; i < prod; ++i) {
		ubig seq_i = 0;
		for (size_t j = 0; j < cnt; ++j) {
			const ubig mod = params[j].mod;
			seq_i += (prod / mod) * seqs[j][i % mod];
		}
		seq_i %= prod;
		out(seq_i);
	}

	for (size_t i = 0; i < cnt; ++i) {
		free(seqs[i]);
	}
	return;
}

ubig seq_len(ubig size, ubig seq[size])
{
	char *seen = malloc(sizeof(char) * size);
	memset(seen, 0, sizeof(char) * size);
	for (size_t i = 0; i < (size_t) size + 1; ++i) {
		if (seen[seq[i]]) {
			return i;
		}
		seen[seq[i]] = 1;
	}
	free(seen);
	return size;
}

#if TEST
static void print_arr(big size, ubig arr[size])
{
	for (size_t i = 0; i < (size_t)size; ++i) {
		printf("%llu ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char** argv)
{
	if (argc != 5) {
		printf("usage: %s <modulus> <multiplier> <adder> <seed>\n",
				argv[0]);
		return 1;
	}

	struct icg_tuple t = {
		strtoull(argv[1], NULL, 10),
		strtoull(argv[2], NULL, 10),
		strtoull(argv[3], NULL, 10),
		strtoull(argv[4], NULL, 10)
	};

	assert(t.mod >= 0 && t.mul >= 0 && t.seed >= 0);
	assert(t.mul < t.mod && t.add < t.mod && t.seed < t.mod);

	ubig *invs = malloc(sizeof(ubig) * t.mod);
	ubig *seq = malloc(sizeof(ubig) * t.mod);

	printf("Got here!\n");

	gen_invs(t.mod, invs);
	gen_seq(t, invs, seq);

	printf("DEBUG: Inverses:\n");
	print_arr(t.mod, invs);
	free(invs);

	printf("DEBUG: Sequence:\n");
	print_arr(t.mod + 1, seq);

	printf("Sequence length: %llu\n", seq_len(t.mod, seq));
	free(seq);
}
#endif
