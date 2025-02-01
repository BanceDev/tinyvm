#include <stdint.h>
#include <stdio.h>

// test program for the tinyvm
// see example.asm for the assembly code
// will be adding an assembler soon so this program will not be needed

int main() {
	uint16_t line1 = 1025;
	uint16_t line2 = 8196;
	uint16_t line3 = 1025;
	uint16_t line4 = 8197;
	uint16_t line5 = 8195;
	uint16_t line6 = 4096;
	uint16_t line7 = 12288;
	uint16_t value = 4;

	FILE *file = fopen("example.bin", "wb");
	if (!file) {
		perror("Failed to open file");
		return 1;
	}

	// Write the lines sequentially
	fwrite(&line1, sizeof(uint16_t), 1, file);
	fwrite(&line2, sizeof(uint16_t), 1, file);
	fwrite(&line3, sizeof(uint16_t), 1, file);
	fwrite(&line4, sizeof(uint16_t), 1, file);
	fwrite(&line5, sizeof(uint16_t), 1, file);
	fwrite(&line6, sizeof(uint16_t), 1, file);
	fwrite(&line7, sizeof(uint16_t), 1, file);

	// Calculate the number of bytes written so far
	long current_pos = ftell(file);
	if (current_pos == -1L) {
		perror("ftell failed");
		fclose(file);
		return 1;
	}

	// Write padding zeros until reaching byte 1025
	uint8_t zero = 0;
	for (long i = current_pos; i < 1025; i++) {
		fwrite(&zero, sizeof(uint8_t), 1, file);
	}

	// Write 'value' at byte 1025
	fwrite(&value, sizeof(uint16_t), 1, file);

	fclose(file);
	return 0;
}
