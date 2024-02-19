#include <iostream>
#include "compiler.hh"

Compiler::Compiler(Lexer *l)
	:instructions(), jump_address()
{
	lexer = l;
}

int Compiler::compile(std::string &err)
{
	Lexer::Token tok = lexer->dqueue_token();
	const Lexer::Token nop;

	size_t current_pos = 0;

	while(tok != nop)
	{
		switch(tok.type)
		{
			case Lexer::PERIOD:
				for(size_t i = 0; i < tok.count; i++)
					instructions.push(static_cast<uint8_t>(VM::i_OUT));
				current_pos += tok.count;
				break;

			case Lexer::COMMA:
				for(size_t i = 0; i < tok.count; i++)
					instructions.push(static_cast<uint8_t>(VM::i_IN));
				current_pos += tok.count;
				break;

			case Lexer::PLUS:
				instructions.push(static_cast<uint8_t>(VM::i_INC_M));
				instructions.push(static_cast<uint8_t>(tok.count & 0xFF));
				current_pos += 2;
				break;

			case Lexer::MINUS:
				instructions.push(static_cast<uint8_t>(VM::i_DEC_M));
				instructions.push(static_cast<uint8_t>(tok.count & 0xFF));
				current_pos += 2;
				break;

			case Lexer::RIGHT:
				instructions.push(static_cast<uint8_t>(VM::i_INC_H));
				instructions.push(static_cast<uint8_t>(tok.count & 0xFF));
				instructions.push(static_cast<uint8_t>((tok.count >> 8) & 0xFF));
				instructions.push(static_cast<uint8_t>((tok.count >> 16) & 0xFF));
				instructions.push(static_cast<uint8_t>((tok.count >> 24) & 0xFF));
				current_pos += 5;
				break;

			case Lexer::LEFT:
				instructions.push(static_cast<uint8_t>(VM::i_DEC_H));
				instructions.push(static_cast<uint8_t>(tok.count & 0xFF));
				instructions.push(static_cast<uint8_t>((tok.count >> 8) & 0xFF));
				instructions.push(static_cast<uint8_t>((tok.count >> 16) & 0xFF));
				instructions.push(static_cast<uint8_t>((tok.count >> 24) & 0xFF));
				current_pos += 5;
				break;

			case Lexer::LSQBRACKET:
				for(size_t i = 0; i < tok.count; i++)
				{
					jump_address.push(current_pos);
					instructions.push(static_cast<uint8_t>(VM::i_JZ));
					instructions.push(0);
					instructions.push(0);
					instructions.push(0);
					instructions.push(0);
					current_pos += 5;
				}
				break;

			case Lexer::RSQBRACKET:
				for(size_t i = 0; i < tok.count; i++)
				{
					size_t prev = 0; 

					if(jump_address.len() > 0)
						prev = jump_address.pop();
					else
					{
						err += "Unmatched Square Brackets.\n";
						return 1;
					}

					uint32_t relative = prev - current_pos; 

					instructions.push(static_cast<uint8_t>(VM::i_JNZ));
					instructions.push(static_cast<uint8_t>(relative & 0xFF));
					instructions.push(static_cast<uint8_t>((relative >> 8) & 0xFF));
					instructions.push(static_cast<uint8_t>((relative >> 16) & 0xFF));
					instructions.push(static_cast<uint8_t>((relative >> 24) & 0xFF));

					current_pos += 5;

					uint32_t *loc = reinterpret_cast<uint32_t *>(instructions.get_data() + prev + 1);
					*loc = -relative;
				}
				break;

			default:
				instructions.push(static_cast<uint8_t>(VM::i_NOP));
				current_pos += 1;
		}

		tok = lexer->dqueue_token();
	}

	err = "Unmatched Square Brackets.\n";
	return jump_address.len() != 0;
}

uint8_t *Compiler::get_instruction_memory()
{
	return instructions.get_data();
}
size_t   Compiler::get_instruction_memory_size()
{
	return instructions.len();
}
