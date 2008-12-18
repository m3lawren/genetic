#include <marshall.h>

#include <vector>

/* Grammar for marshalled output:
 *
 * INT ::= -?[0-9]+
 *
 * 1.   S        -> "(" CONFIG "," DNA ")"
 * 2.   CONFIG   -> "(" PAIR PAIRLIST ")"
 * 3.   PAIRLIST -> "," PAIR PAIRLIST
 * 4.   PAIRLIST -> $
 * 5.   PAIR     -> "{" INT "," INT "}"
 * 6.   DNA      -> "(" ELEM ELEMLIST ")"
 * 7.   ELEMLIST -> "," ELEM ELEMLIST
 * 8    ELEMLIST -> $
 * 9.   ELEM     -> "{" INT INTLIST "}"
 * 10.  INTLIST  -> "," INT INTLIST
 * 11.  INTLIST  -> $
 *
 * State    | "(" | ")" | "{" | "}" | "," | INT |  $
 * ---------+-----+-----+-----+-----+-----+-----+----
 * S        |  1  |  -  |  -  |  -  |  -  |  -  |  -
 * CONFIG   |  2  |  -  |  -  |  -  |  -  |  -  |  -
 * PAIRLIST |  -  |  -  |  -  |  -  |  3  |  -  |  4
 * PAIR     |  -  |  -  |  5  |  -  |  -  |  -  |  -
 * DNA      |  6  |  -  |  -  |  -  |  -  |  -  |  -
 * ELEMLIST |  -  |  -  |  -  |  -  |  7  |  -  |  8
 * ELEM     |  -  |  -  |  9  |  -  |  -  |  -  |  -
 * INTLIST  |  -  |  -  |  -  |  -  | 10  |  -  | 11
 */

struct Token {
	std::string  value;
	enum { 
		LPAREN,
		RPAREN,
		LBRACE,
		RBRACE,
		COMMA,
		INT,
		EOT,
	} type;
};

const char* parseInt(const char* str, Token&) {
	return str;
}

std::vector<Token> tokenize(const char* str) {
	std::vector<Token> tokens;
	const char* orig = str;
	while (*str) {
		Token t;
		switch (*str) {
			case '(':
				t.value = *str++;
				t.type = Token::LPAREN;
				break;
			case ')':
				t.value = *str++;
				t.type = Token::RPAREN;
				break;
			case '{':
				t.value = *str++;
				t.type = Token::LBRACE;
				break;
			case '}':
				t.value = *str++;
				t.type = Token::RBRACE;
				break;
			case ',':
				t.value = *str++;
				t.type = Token::COMMA;
				break;
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				const char* nstr = parseInt(str, t);
				if (!nstr) {
					throw ParseError("Invalid integer value.", str - orig);
				}
				str = nstr;
				break;
			}
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				str++;	
				break;
			default:
				throw ParseError("Unrecognized character.", str - orig);
		}
	}
	return tokens;
}
