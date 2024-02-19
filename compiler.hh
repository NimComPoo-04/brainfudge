#ifndef _COMPILER_HH_
#define _COMPILER_HH_

#include <string>
#include "lexer.hh"
#include "vm.hh"
#include "Vector.hh"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed int int32_t;

class Compiler
{
private:
	Lexer *lexer;

	Vector<uint8_t> instructions;
	Vector<uint32_t> jump_address;

public:
	Compiler(Lexer *l);
	int compile(std::string &err);
	std::string decompile();

	uint8_t *get_instruction_memory();
	size_t   get_instruction_memory_size();
};

#endif
