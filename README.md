# BrainFudge

An Interpreter for the BrainFudge Programming language.

As the inventor of this programming language
describe it to be. This is a pure turing machine controller.

So ig harvard architecture?

Exposed Registers:

I	instruction pointer
H	head register

instruction pointer is incremented each time until
head points to a location in memory thats it ig.

Memory:

theoretically infinite, in our case its practically infinite
but definitely not infinite if ya know what i mean.
Extends both ways in +ve and -ve

Instructions:

2 byte instructions

OUT imm	generates an _hardware interupt_ that tells the
	io chip to take imm values from H and draw it on screen
	then increment the position of the cursor each time

<u>not implemented for now</u>

IN  imm generates and _hardware interupt_ that tells the
	io chip, which then halts the CPU and waits for input.

3 byte instruction

INC H imm		increment with an immidate value
DEC H imm		decrement with an immidate value

INC [H] imm		increment with an immidate value
DEC [H] imm		decrement with an immidate value

multibyte instruction

JZ8, JZ16, JZ32 imm    	conditional relative jump if current
			memory pointed to by Head is
			zero jump

JNZ8, JNZ16, JNZ32 imm    conditional relative jump if current
			  memory pointed to the Head is
			  not zero
