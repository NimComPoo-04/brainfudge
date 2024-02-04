#include <iostream>
#include <fstream>
#include <cstdlib>
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
	else
	{
		cerr << "WRONG FILE???????" << endl;
		return 1;
	}
	f.close();

	Lexer l(source, 0);
	if(l.tokenize(err) != 0)
	{
		cerr << err;
		return 1;
	}

	err = "";
	Compiler c(&l);
	if(c.compile(err) != 0)
	{
		cerr << err;
		return 1;
	}

	VM::InstructionMemory rom(c.get_instruction_memory(), c.get_instruction_memory_size());
	VM::DataMemory ram;

	VM::Cpu cpu(ram, rom);

	string value(argc > 2 ? argv[2] : "0");

	if(value == "1")
		Controller::programRun(cpu, 1);
	else if(value == "2")
		Controller::programRun(cpu, 2);
	else
		Controller::programRun(cpu, 0);

	return 0;
}
