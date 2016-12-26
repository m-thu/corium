/*
 * Invocation: clear
 *
 * Clear screen
 *
 * Return value: always 0
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	/* clears VT100 terminal */
	write(1, "\x1b[H\x1b[J", 6);

	return EXIT_SUCCESS;
}
