#include <getopt.h> /* getopt() */
#include <stdio.h>
#include "icg.h"

int main(int argc, char **argv)
{
	struct icg_tuple params[2] = {
		{8191, 98, 7430, 4306},		/* period 8191 */
		{8179, 92, 6846, 3906}		/* period 8190 */
		//{4097, 3840, 4056, 3082},	/* period 4097 */
		//{4095, 2534, 120, 1024}	/* period 4095 */
	}; /* Note, all periods are coprime. */
	/* period = 8191 * 8190 * 4097 * 4095 */
	/* state size = sizeof(ubig) * period */

	int ch = -1, binary_mode = 0;
	ubig period = 1, *seq;
	size_t param_len = sizeof(params) / sizeof(params[0]);
	if (argc != 2) {
		printf("Usage: %s -{a, b, l} for ascii and binary output "
			"respectively.\n-l is a special option that prints the "
			"length of the generated sequence, assuming you have "
			"enough memory to store it, that is.\n", argv[0]);
		return 1;
	}
	while ((ch = getopt(argc, argv, "abl")) != -1) {
		switch(ch) {
		case 'b':
			binary_mode = 1;
			break;
		case 'a':
			binary_mode = 0;
			break;
		case 'l':
			for (size_t i = 0; i < param_len; ++i) {
				period *= params[i].mod;
			}
			seq = cicg(params, param_len);
			assert(seq != NULL);
			printf("sequence length: %llu\n", seq_len(period, seq));
			free(seq);
			return 0;
		default:
			break;
		}
	}
	cicg_writer(params, param_len, binary_mode);
	return 0;
}
