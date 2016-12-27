/*
 * Invocation: reset
 *
 * Reset terminal
 *
 * Return value: always 0
 *
 */

#include "lib.h"

#define ESC "\x1b"

int main()
{
	write(1, ESC"c" ESC"(B" ESC"[0m" ESC"[J" ESC"[?25h", 18);

	return EXIT_SUCCESS;
}
