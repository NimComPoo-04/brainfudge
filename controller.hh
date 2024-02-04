#ifndef _CONTROLLER_HH_
#define _CONTROLLER_HH_

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
