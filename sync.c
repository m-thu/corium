/*
 * Invocation: sync
 *
 * Flush filesystem buffers
 *
 * Return value: always 0
 *
 */

#include "lib.h"

int main()
{
	sync();

	return EXIT_SUCCESS;
}
