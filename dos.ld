/* Linker script for DOS com executables */

OUTPUT_FORMAT("binary")
SECTIONS
{
	. = 0x100;

	.text : {
		* (.text);
	}

	.data : {
		* (.data);
	}

	.rodata : {
		* (.rodata);
	}

	.bss : {
		* (.bss);
	}
}
