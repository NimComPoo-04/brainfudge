#ifndef _LEXER_HH_
#define _LEXER_HH_

#include <string>
#include "Vector.hh"
// #include <vector>
//
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// Run length encodes the source file
class Lexer
{
public:
	enum TokenType
	{
		NOB = 0,		// the no token
		RIGHT = '>',
		LEFT = '<',
		PLUS = '+',
		MINUS = '-',
		PERIOD = '.',
		COMMA = ',',
		LSQBRACKET = '[',
		RSQBRACKET = ']'
	};

	class Token
	{
	private:
		TokenType type;
		size_t count;

	public:
		Token();
		Token(TokenType type, size_t count);

		bool operator==(const Token &other) const;
		bool operator!=(const Token &other) const;

		// a printable string
		operator std::string() const;

		friend class Lexer;
		friend class Compiler;
	};

private:
	std::string source;
	size_t current; 	// current token

	// std::vector<Token> tokens;	// tokenized list of values
	//
	// TODO: use the standard vector instead of this,
	// but in this case we wanna learn so this is fine
	Token *tokens;
	size_t base_ptr;
	size_t length;
	size_t capacity;

private:
	// Specialized for this perpose.
	void enqueue_token(Token t);

public:
	Lexer(const std::string &value, int current);

	// returns exit code incase of error
	int tokenize(std::string &error_str); 

	Token next();
	Token dqueue_token();

	~Lexer();
}; 

#endif
