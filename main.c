#include "icg.c"

int main(int argc, char** argv) {
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
