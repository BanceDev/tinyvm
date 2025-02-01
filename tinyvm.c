/*
Copyright (C) 2025 Lance Borden

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3.0
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.

*/
#include <stdint.h>
#include <stdio.h>

//===============================================================
// Memory and Registers
//===============================================================

// 12 bit byte addressing so 4Kb of ROM
// first 1Kb of ROM is reserved for executable
// Word size is 16 bits
#define MAX_ROM 1 << 12
uint8_t rom[MAX_ROM];

enum {
	R_PC = 0, // program counter
	R_IR,	  // instruction register
	R_MAR,	  // memory address register
	R_MDR,	  // memory data register
	R_X,	  // index register for operation with AC
	R_AC,	  // accumulator register
	R_SIZE	  // number of registers
};

uint16_t registers[R_SIZE];

//===============================================================
// Instruction Set
//===============================================================

enum { ADD_ABS = 0, ADD_IDR, JMP, END };

// helper method for both types of addition
static void add() {
	// read add operand to data register
	registers[R_MDR] = rom[registers[R_MAR]] + (rom[registers[R_MAR] + 1] << 8);
	// put operand into X
	registers[R_X] = registers[R_MDR];
	// perform addition via "ALU" to accumulator
	registers[R_AC] += registers[R_X];
}

// add instruction for absolute addressing: ADD addr
static void add_absolute() {
	// addr will be the last 12 bits in the IR
	registers[R_MAR] = registers[R_IR] & 0x0FFF;
	add();
}

// add instruction for indirect addressing: ADD (addr)
static void add_indirect() {
	// addr will be the last 12 bits in the IR
	registers[R_MAR] = registers[R_IR] & 0x0FFF;
	// read effective address from ROM
	registers[R_MDR] = rom[registers[R_MAR]] + (rom[registers[R_MAR] + 1] << 8);
	// put the EA from MDR into MAR
	registers[R_MAR] = registers[R_MDR];
	add();
}

// jump instruction: JMP addr
static void jump() {
	// addr will be in the last 12 bits of the IR
	// PC has to always be 2x since it should
	// be at the start of an instruction word
	registers[R_PC] = (registers[R_IR] & 0x0FFF) * 2;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Execution of tinyvm needs a ROM arg to run\n");
		return 1;
	}
	// recieve ROM binary from command line
	FILE *rom_file = fopen(argv[1], "rb");
	if (!rom_file) {
		perror("Failed to open ROM binary");
		return 1;
	}

	// put file contents into vm ROM
	size_t rom_bytes = fread(rom, 1, sizeof(rom), rom_file);
	if (rom_bytes > 0) {
		printf("Read %zu bytes into tinyVM ROM\n", rom_bytes);
	}

	fclose(rom_file);

	// reset PC
	registers[R_PC] = 0;
	// opcode is highest 4 bits
	uint8_t opcode;
	while (registers[R_PC] < 1024 && opcode != END) {
		// the second byte is the high byte due to x86 being little endian
		registers[R_IR] =
			rom[registers[R_PC]] + (rom[registers[R_PC] + 1] << 8);
		opcode = registers[R_IR] >> 12;
		switch (opcode) {
		case ADD_ABS:
			add_absolute();
			break;
		case ADD_IDR:
			add_indirect();
			break;
		case JMP:
			jump();
			break;
		default:
			break;
		}
		registers[R_PC] += 2;
	}

	// perform execution logging of all registers
	printf("TinyVM Post-Execution Register Contents:\n");
	printf("PC: %d\n", registers[R_PC]);
	printf("IR: %d\n", registers[R_IR]);
	printf("MAR: %d\n", registers[R_MAR]);
	printf("MDR: %d\n", registers[R_MDR]);
	printf("X: %d\n", registers[R_X]);
	printf("AC: %d\n", registers[R_AC]);

	return 0;
}
