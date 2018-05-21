#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stack>
#include <string>

#include <boost/algorithm/string.hpp>

namespace engine2
{

using namespace std;

/**
 * @brief Quote and enclosure handling tokenizer
 */
class Tokenizer
{
public:
	explicit Tokenizer(char e = '\\',
	                   char s = ',',
	                   char q = '"');
	
	Tokenizer(string e, string s, string q);
	Tokenizer(string e, string s, string qs, string qe);
	
	template<class Iterator, class Token>
	bool operator()(Iterator &next, Iterator &end, Token &tok);
	
	void reset();
	
	static Tokenizer csv_tk; ///< Comma-separated values
	static Tokenizer ssv_tk; ///< Space-separated values
	
private:
	string escape_c;
	string separator_c;
	string quote_start_c;
	string quote_end_c;
	
	bool is_escape(char c);
	bool is_separator(char c);
	bool is_quote_start(char c);
	bool is_quote_end(char c, char t);
};

template<class Iterator, class Token>
bool Tokenizer::operator()(Iterator &next, Iterator &end, Token &tok)
{
	if (next == end) { return false; }
	
	stack<char> quote_stack;
	
	tok = Token();
	
	for (; next != end; ++next) {
		if (is_escape(*next)) {
			tok += *next;
			tok += *(++next);
		}
		else if (quote_stack.size() > 0 && is_quote_end(*next, quote_stack.top())) {
			tok += *next;
			quote_stack.pop();
		}
		else if (is_quote_start(*next)) {
			tok += *next;
			quote_stack.push(*next);
		}
		else if (quote_stack.size() == 0 && is_separator(*next)) {
			++next;
			boost::trim(tok);
			return true;
		}
		else {
			tok += *next;
		}
	}
	
	boost::trim(tok);
	return true;
}

}

#endif // INITOKENIZER_H
