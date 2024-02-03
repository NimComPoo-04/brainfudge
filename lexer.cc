#include "lexer.hh"

Lexer::Lexer(const std::string &value, int current = 0)
{
	this->source = value;
	this->current = current;

	// Lazy load them don't bother
	this->tokens = 0;
	this->length = 0;
	this->capacity = 0;
	this->base_ptr = 0;
}

void Lexer::enqueue_token(Lexer::Token t)
{
	if(this->length >= this->capacity)
	{
		// Renormalizing the capacity and the length
		// since the array is getting reallocated
		int len = (-this->base_ptr + this->length) * 1.5;
		this->capacity = (len < 256 ? 256 : len);

		// Copying the relevant old data into the new buffer
		auto *octavious = new Lexer::Token[this->capacity];
		for(size_t i = this->base_ptr, j = 0; i < this->length; i++, j++)
			octavious[j] = this->tokens[i];
		delete[] this->tokens;

		// Reassigning the tokens array
		this->tokens = octavious;
		this->length -= this->base_ptr;
		this->base_ptr = 0;
	}

	this->tokens[this->length++] = t;
}

Lexer::Token Lexer::dqueue_token()
{
	if(this->base_ptr >= this->length)
	{
		// most defenitely out of tokens
		return Lexer::Token();
	}
	else
	{
		// normally return whatever is required
		return this->tokens[this->base_ptr++];
	}
}

Lexer::~Lexer()
{
	delete[] tokens;
}

Lexer::Token::Token(Lexer::TokenType type,size_t count)
{
	Lexer::Token::type = type;
	Lexer::Token::count = count;
}

bool Lexer::Token::operator==(const Token &other) const
{
	return (other.type == this->type) and (other.count == this->count);
}

bool Lexer::Token::operator!=(const Token &other) const
{
	return !(other == *this);
}

Lexer::Token::operator std::string() const
{
	std::string s = "( ";
	s += static_cast<char>(this->type);
	s += " : ";
	s += std::to_string(this->count);
	s += " )";

	return s;
}

int Lexer::tokenize(std::string &error_str)
{
	Lexer::Token tok = next();
	Lexer::Token nop;

	while(tok != nop)
	{
		error_str += std::string(tok);
		enqueue_token(tok);
		tok = next();
	}

	return 0;
}

Lexer::Token Lexer::next()
{
	const std::string valid = "><+-.,[]";
	Lexer::Token tok;
	while(current < source.size())
	{
		switch(static_cast<Lexer::TokenType>(source[current]))
		{
			case Lexer::TokenType::RIGHT: case Lexer::TokenType::LEFT:
			case Lexer::TokenType::PLUS: case Lexer::TokenType::MINUS:
			case Lexer::TokenType::PERIOD: case Lexer::TokenType::COMMA:
			case Lexer::TokenType::LSQBRACKET: case Lexer::TokenType::RSQBRACKET:
				{
					tok.type = static_cast<TokenType>(source[current]);
					tok.count = current;
					int c = source[current];
					size_t vec = 0;
					size_t count = 0;
					while(current < source.size())
					{
						vec = valid.rfind(source[current]);
						if(source[current] != c)
							break;
						current++;
						if(vec == std::string::npos)
							continue;
						count++;
					}
					tok.count = count;
				}
				goto OUT;

			default:
				current++;
		}
	}
OUT:
	return tok;
}

Lexer::Token::Token()
{
	this->type = Lexer::TokenType::NOB;
	this->count = 0;
}
