#include <iostream>
#include <fstream>

#include <cassert>
#include "compiler.hh"
using namespace std;

#include "lexer.hh"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cerr << "Usage: ./bf [filename]\n";
		return 1;
	}

	string err;
	string source;

	fstream f(argv[1]);
	if(f.is_open())
	{
		f.seekg(ios::end);
		source.reserve(f.tellg());
		f.seekg(ios::beg);
		source.assign(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
	}
	f.close();

	Lexer l(source, 0);
	assert(l.tokenize(err) == 0);

	err = "";
	Compiler c(&l);
	assert(c.compile(err) == 0);

	VM::InstructionMemory rom(c.get_instruction_memory(), c.get_instruction_memory_size());
	VM::DataMemory ram;

	VM::Cpu cpu(ram, rom);

	Controller::programRun(cpu, 0);

	return 0;
}
