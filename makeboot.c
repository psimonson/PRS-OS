/*
 * Simple program to copy a boot sector to a floppy disk.
 * Author: Philip R. Simonson
 * Date  : July 13, 2019
 */

#include <stdio.h>
#include "file.h"

/* Entry point for program.
 */
int main(int argc, char **argv)
{
	/* file struct */
	file_t file, file2;
	int c;

	if(argc < 1 || argc > 2) {
		printf("Usage: %s <filename | device>\n", argv[0]);
		return 2;
	}
	init_file(&file);
	open_file(&file, "boot.bin", "rb");
	if(get_errori_file(&file) != FILE_ERROR_OKAY) {
		printf("Error: opening file for reading.\n");
		return 1;
	}
	init_file(&file2);
	open_file(&file2, argv[1], "wb");
	if(get_errori_file(&file2) != FILE_ERROR_OKAY) {
		printf("Error: opening file for writing.\n");
		return 1;
	}
	while((c = getc_file(&file)) != EOF) {
		putc_file(&file2, c);
	}
	if(get_errori_file(&file) != FILE_ERROR_OKAY)
		return 1;
	if(get_errori_file(&file2) != FILE_ERROR_OKAY)
		return 2;
	close_file(&file);
	close_file(&file2);
	return 0;
}
