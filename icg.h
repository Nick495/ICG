#ifndef ICG_H_
#define ICG_H_

#include <stdio.h>	/* printf() */
#include <stdlib.h>	/* strtoull() */
#include <string.h>	/* memset */
#include <unistd.h>	/* write() */
#include <assert.h>	/* assert() */

typedef long long big;
typedef unsigned long long ubig;

struct icg_tuple {
	big mod;
	big mul;
	big add;
	big seed;
};

/* Note: The returned pointer must be free()'d. */
ubig *icg(struct icg_tuple args);
ubig *cicg(struct icg_tuple *params, size_t cnt);

/* Note: Whether to output in binary mode or not (ascii). */
void cicg_writer(struct icg_tuple *params, size_t cnt, int binary_mode);

ubig seq_len(ubig size, ubig seq[size]);

#endif
