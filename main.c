#include "icg.h"

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

	gen_invs(t.mod, invs);
	gen_seq(t, invs, seq);

	printf("DEBUG: Inverses:\n");
	print_arr(t.mod, invs);
	free(invs);

	printf("DEBUG: Sequence:\n");
	print_arr(t.mod, seq);

	printf("Sequence length: %llu\n", seq_len(t.mod, seq));
	free(seq);
}
