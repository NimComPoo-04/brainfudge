# BrainFudge

Implementation of the very simple BrainFudge Langauge.
This language is identical to Brainfuck which I have no problem in mentioning however
it does seem that it is not a very good name for formal situations.
Created by Brainfuck is an esoteric programming language created in 1993 by Urban Müller.
BrainFudge is probably the simplest programming langauge to implement a interpreter for.

We compile the brain fudge language and compile it to a custom bytecode.
The information recieved is runlength encoded to effectivly reduce the
number of istructions. We also employ the optimization where if there
are more than 256 additions we constant fold a part of the operation, since
in our implementation the memory is byte addressible.

The virtual machine itself has 2 registers, the
instruction pointer(I) and memory address register(H).
Both are 32 bit in size.

It contains 2 different kinds of memory, Instruction memory which not mutated during the
runtime of the program, this memory is linear in nature and  Data memory which is paged into
256 byte chunks. The reason for paging the data is memory is because in theory we have a infinite
memory on a very very long tape (turing machine!!!). This is however impossible to do for a little
computer. To allow the programs to access blocks far far far away from the original location
and even negetive memory locations.

| Instruction | Description | Size (in bytes) |
| ---         | ---         | --- |
| NOP         | No operation | 1 |
| OUT         | Puts the value of [H] onto the screen as a ASCII character | 1 |
| IN          | Accepts a single character input from the user | 1 |
| INC_M       | Increment the value of [H] by immidiate value specified | 2 |
| DEC_M       | Decrement the value of [H] by immidiate value specified | 2 |
| INC_H       | Increment the value of H register by immidiate value specified | 5 |
| DEC_H       | Decrement the value of H register by immidiate value specified | 5 |
| JZ          | Jump to a parituclar memory location if [H] is zero | 5 |
| JNZ         | Jump to a particular memory location if [H] is not zero | 5 |
| HALT        | Halts the program (actually an error which happens for out of bounds access on instruction memory) | 1 |

It would be ideal to have some sort of event based io here since brainf does not really deal with any sort of buffers.
But I am not aware of any crossplatform solutions to this specific problem which does not require extern libraries.

## References

* __Basic Information on the language, its syntax and spec__
	* [Wikipedia](https://en.wikipedia.org/wiki/Brainfuck)
	* [Esolang](https://esolangs.org/wiki/Brainfuck)


* __All Tests and other useful info__
	* Programs that are in the test folder are mostly taken from this repository by [Rdebath](https://github.com/rdebath/Brainfuck/)
	* Also very useful bf programs for testing the interpreter by [saulpw](https://github.com/saulpw/brainfuck/)
	* The abc.bf was written by me to test some important things.
