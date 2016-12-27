/*
 * Invocation: clear
 *
 * Clear screen
 *
 * Return value: always 0
 *
 */

#include "lib.h"

#define ESC "\x1b"

int main()
{
	/* clear VT100 terminal */
	write(1, ESC"[H" ESC"[J", 6);

	return EXIT_SUCCESS;
}
