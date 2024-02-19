#ifndef _CONTROLLER_HH_
#define _CONTROLLER_HH_

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;

namespace VM
{
	class DataMemory;
	class InstructionMemory;
	class Cpu;
}

namespace Controller
{
	void memoryDump(VM::DataMemory &);
	void memoryDump(VM::InstructionMemory &, size_t start = 0, size_t count = -1);
	void cpuStatusDump(VM::Cpu &);

	int programStep(VM::Cpu &);
	void programRun(VM::Cpu &, int debug);

	extern int TICK_SPEED;
}

#endif
