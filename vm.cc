#include <cstdio> // IOSTREAM is too slow for this
#include "vm.hh"

namespace VM
{
	int Cpu::NOP() { return 1; }

	int Cpu::INC_H(int32_t a)
	{
		H += a;
		return 1;
	}

	int Cpu::DEC_H(int32_t a)
	{
		H -= a;
		return 1;
	}

	int Cpu::INC_M(int8_t a)
	{
		data_memory[H] += a;
		return 2;
	}

	int Cpu::DEC_M(int8_t a)
	{
		data_memory[H] -= a;
		return 2;
	}

	int Cpu::JZ(int32_t r)
	{
		if(data_memory[H] == 0)
		{
			I += r;
			return 2;
		}
		else
			return 1;
	}

	int Cpu::JNZ(int32_t r)
	{
		if(data_memory[H] != 0)
		{
			I += r;
			return 2;
		}
		else
			return 1;
	}

	int Cpu::OUT()
	{
		putchar(static_cast<char>(data_memory[H]));
		return 3;
	}

	int Cpu::IN()
	{
		data_memory[H] = static_cast<int8_t>(getchar());
		return 3;
	}

	Cpu::Cpu(DataMemory &d, InstructionMemory &i)
		:data_memory(d), ins_memory(i)
	{
		I = 0;
		H = 0;
	}

	DataMemory::AssocCell::AssocCell(int32_t arg)
	{
		argument = arg;
		next = 0;
		for(int i = 0; i < 256; i++)
			page[i] = 0;
	}

	DataMemory::DataMemory()
	{
		pages = new AssocCell(0);
	}

	DataMemory::~DataMemory()
	{
		while(pages != 0)
		{
			AssocCell *k = pages->next;
			delete pages;
			pages = k;
		}
	}

	int8_t &DataMemory::operator[](int32_t indx)
	{
		int32_t arg = static_cast<uint32_t>(indx) >> 8;
		int32_t index = indx & 0xff;

		AssocCell *k = pages;
		AssocCell *prev = 0;
		while(k)
		{
			if(k->argument == arg)
			{
				return k->page[index];
			}
			prev = k;
			k = k->next;
		}

		prev->next = new AssocCell(arg);
		return prev->next->page[index];
	}

	InstructionMemory::InstructionMemory(uint8_t *buf, size_t s)
	{
		buffer = buf;
		size = s;
	}

	uint8_t InstructionMemory::operator[](uint32_t index) const
	{
		if(index > size)
			return static_cast<uint8_t>(i_HALT);
		return buffer[index];
	}

	int Cpu::fetch_decode_execute()
	{
		int ticks = 0;

		// Fetching instructions
		switch(static_cast<Instruction>(ins_memory[I]))
		{
			case i_HALT:
				return 0;
			case i_NOP:
				{
					I += 1;
					ticks = NOP();
				}
				break;

			case i_OUT:
				{
					I += 1;

					ticks = 1;
					ticks += OUT();
				}
				break;

			case i_IN:
				{
					I += 1;

					ticks = 1;
					ticks += IN();
				}
				break;

			case i_INC_M:
				{
					uint8_t operand = ins_memory[I+1];
					I += 2;

					ticks = 2;
					ticks += INC_M(operand);
				}
				break;

			case i_DEC_M:
				{
					uint8_t operand = ins_memory[I+1];
					I += 2;

					ticks = 2;
					ticks += DEC_M(operand);
				}
				break;

			case i_INC_H:
				{
					uint32_t operand = 0;
					operand = (operand << 8) | ins_memory[I+4];
					operand = (operand << 8) | ins_memory[I+3];
					operand = (operand << 8) | ins_memory[I+2];
					operand = (operand << 8) | ins_memory[I+1];
					I += 5;

					ticks = 5;
					ticks += INC_H(operand);
				}
				break;

			case i_DEC_H:
				{
					uint32_t operand = 0;
					operand = (operand << 8) | ins_memory[I+4];
					operand = (operand << 8) | ins_memory[I+3];
					operand = (operand << 8) | ins_memory[I+2];
					operand = (operand << 8) | ins_memory[I+1];
					I += 5;

					ticks = 5;
					ticks += DEC_H(operand);
				}
				break;

			case i_JZ:
				{
					uint32_t operand = 0;
					operand = (operand << 8) | ins_memory[I+4];
					operand = (operand << 8) | ins_memory[I+3];
					operand = (operand << 8) | ins_memory[I+2];
					operand = (operand << 8) | ins_memory[I+1];
					I += 5;

					ticks = 5;
					ticks += JZ(operand);
				}
				break;

			case i_JNZ:
				{
					uint32_t operand = 0;
					operand = (operand << 8) | ins_memory[I+4];
					operand = (operand << 8) | ins_memory[I+3];
					operand = (operand << 8) | ins_memory[I+2];
					operand = (operand << 8) | ins_memory[I+1];
					I += 5;

					ticks = 5;
					ticks += JNZ(operand);
				}
				break;
		}

		return ticks;
	}
}
