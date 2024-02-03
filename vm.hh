#ifndef _VM_HH_
#define _VM_HH_

#include <cinttypes>

namespace VM
{
	enum class Instruction : uint8_t
	{
		NOP,
		OUT, IN,
		INC_H, DEC_H,
		INC_M, DEC_M,
		JZ, JNZ,
		HALT
	};

	// Paged memory nothing weird
	class DataMemory
	{
	private:
		// NOTE: This is a fine because
		// it is very unlikely that a lot
		// of pages will be allocated
		struct AssocCell
		{
		public:
			int32_t argument;
			int8_t page[256];

			AssocCell *next;

		public:
			AssocCell(int32_t);
			
		};

		AssocCell *pages;

	public:
		DataMemory();
		~DataMemory();
		int8_t &operator[](int32_t);
	};

	// Linear Memory nothing weird
	class InstructionMemory
	{
	private:
		uint8_t *buffer;
		size_t size;

	public:
		InstructionMemory(uint8_t *, size_t);
		uint8_t operator[](uint32_t) const;
	};

	class Cpu
	{
	public:
		uint32_t I;	// instruction pointer
		int32_t H;	// head register

		DataMemory &data_memory;
		InstructionMemory &ins_memory;
		
	public:
		int NOP();

		int OUT();
		int IN();

		int INC_H(int32_t);
		int DEC_H(int32_t);
		int INC_M(int8_t);
		int DEC_M(int8_t);

		int JZ(int32_t);
		int JNZ(int32_t);

	public:
		int fetch_decode_execute();
		Cpu(DataMemory &, InstructionMemory &);

		void memorydump(void);
	};
}

#endif
