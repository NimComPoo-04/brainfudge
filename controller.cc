#include <iostream>
#include <iomanip>

#include "controller.hh"
#include "vm.hh"

namespace Controller
{
	int TICK_SPEED = 120;

	void memoryDump(VM::DataMemory &mem)
	{
		using namespace std;

		VM::DataMemory::AssocCell *a = mem.pages;

		while(a)
		{
			cerr << "Page: " << setfill('0') << setw(6) << hex << noshowbase << a->argument << endl;
			cerr << "Values: " <<endl;
			for(size_t i = 0, k = 0; i < 16; i++)
			{
				for(size_t j = 0; j < 16; j++)
				{
					cerr << setfill('0') << setw(2) << left << hex << noshowbase << ((uint16_t)a->page[k++] & 0xFF) << " ";
				}
				cerr << endl;
			}

			a = a->next;
		}
	}

	void memoryDump(VM::InstructionMemory &ins, size_t start, size_t count)
	{
		using namespace std;

		const uint8_t *instructions = ins.buffer;

		for(size_t i = start; i < ins.size && count; i++)
		{
			cerr << setfill(' ') << setw(15) << left << hex << noshowbase << i << dec;

			switch(static_cast<VM::Instruction>(instructions[i]))
			{
				case VM::i_OUT:
					cerr << "OUT";
					break;

				case VM::i_IN:
					cerr << "IN";
					break;

				case VM::i_INC_M:
					cerr << "INC *H\t" << hex << noshowbase << (int)instructions[++i];
					break;
				case VM::i_DEC_M:
					cerr << "DEC *H\t" << hex << noshowbase << (int)instructions[++i];
					break;

				case VM::i_INC_H:
					{
						uint32_t value = 0;
						value = (value << 8) | instructions[i + 4];
						value = (value << 8) | instructions[i + 3];
						value = (value << 8) | instructions[i + 2];
						value = (value << 8) | instructions[i + 1];
						i += 4;

						cerr << "INC  H\t" << hex << noshowbase << value;
					}
					break;
				case VM::i_DEC_H:
					{
						uint32_t value = 0;
						value = (value << 8) | instructions[i + 4];
						value = (value << 8) | instructions[i + 3];
						value = (value << 8) | instructions[i + 2];
						value = (value << 8) | instructions[i + 1];
						i += 4;

						cerr << "DEC  H\t" << hex << noshowbase << value;
					}
					break;
				case VM::i_JZ:
					{
						uint32_t value = 0;
						value = (value << 8) | instructions[i + 4];
						value = (value << 8) | instructions[i + 3];
						value = (value << 8) | instructions[i + 2];
						value = (value << 8) | instructions[i + 1];
						i += 4;

						cerr << "JZ\t" << hex << noshowbase << (int)(value + i + 1);
					}
					break;
				case VM::i_JNZ:
					{
						uint32_t value = 0;
						value = (value << 8) | instructions[i + 4];
						value = (value << 8) | instructions[i + 3];
						value = (value << 8) | instructions[i + 2];
						value = (value << 8) | instructions[i + 1];
						i += 4;

						cerr << "JNZ\t" << hex << noshowbase << (int)(value + i + 1);
					}
					break;

				default:
					cerr << "NOP";
			}

			count--;
			cerr << endl;
		}
	}

	void cpuStatusDump(VM::Cpu &cpu)
	{
		using namespace std;

		cerr << "I: " << hex << cpu.I
			<< " H: " << hex << cpu.H
			<< " [H]: " << hex << (int)cpu.data_memory[cpu.H]
			<< endl;
	}

	int programStep(VM::Cpu &cpu)
	{
		return cpu.fetch_decode_execute();
	}

	void programRun(VM::Cpu &cpu, int debug = 0)
	{
		using namespace std;
		if(debug == 1)
		{
			int k = 1;
			cerr << "Debugging: " << endl;
			cerr << "Commands: " << endl;
			cerr << "src          : prints byte-code" << endl; 
			cerr << "ins [number] : prints byte-code at" << endl; 
			cerr << "brk [number] : runs until a certain ins no." << endl; 
			cerr << "pgs          : prints all the pages of allocated memory." << endl; 
			cerr << "mem [number] : prints value of memory at." << endl; 
			cerr << "stp          : steps one instructions ahead" << endl; 

			while(k && cin.good())
			{
				string input;
				cerr << "(bf) ";
				cin >> input;

				if(input == "src")
				{
					memoryDump(cpu.ins_memory);
				}
				else if(input == "ins")
				{
					size_t ins_no;
					cin >> hex >> ins_no;

					memoryDump(cpu.ins_memory, ins_no, 5);
				}
				else if(input == "brk")
				{
					size_t at;
					cin >> hex >> at;

					while(cpu.I < at && (k = programStep(cpu)));
					memoryDump(cpu.ins_memory, cpu.I, 5);
				}
				else if(input == "pgs")
				{
					memoryDump(cpu.data_memory);
				}
				else if(input == "mem")
				{
					size_t at;
					cin >> hex >> at;

					cerr << "[" << hex << at << "] " << hex << setfill('0') << setw(2)
						<< ((int)(cpu.data_memory[at]) & 0xFF) << endl;
				}
				else if(input == "stp")
				{
					k = programStep(cpu);
					cpuStatusDump(cpu);
				}
				else
				{
					cerr << "Invalid Command" << endl;
				}
			}
			cerr << "Program Stopped." << endl;
		}
		else
		{
			while(programStep(cpu));
			if(debug == 2)
			{
				cerr << "== Instruction Memory Dump ==" << endl;
				memoryDump(cpu.ins_memory);

				cerr << "== Data Memory Dump ==" << endl;
				memoryDump(cpu.data_memory);

				cerr << endl;
			}
		}
	}
}
