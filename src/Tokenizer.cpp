#include "Util/Tokenizer.h"

namespace engine2
{

Tokenizer::Tokenizer(char e, char s, char q)
    : escape_c(1,e), separator_c(1,s), quote_start_c(1,q), quote_end_c(1,q)
{ }

Tokenizer::Tokenizer(string e, string s, string q)
    : escape_c(e), separator_c(s), quote_start_c(q), quote_end_c(q)
{ }

Tokenizer::Tokenizer(string e, string s, string qs, string qe)
    : escape_c(e), separator_c(s), quote_start_c(qs), quote_end_c(qe)
{ }

bool Tokenizer::is_escape(char c)
{ return (escape_c.find(c) != string::npos); }
bool Tokenizer::is_separator(char c)
{ return (separator_c.find(c) != string::npos); }
bool Tokenizer::is_quote_start(char c)
{ return (quote_start_c.find(c) != string::npos); }
bool Tokenizer::is_quote_end(char c, char t)
{
	return (   quote_end_c.find(c) == quote_start_c.find(t)
	        && quote_end_c.find(c) != string::npos);
}

void Tokenizer::reset() { }

Tokenizer Tokenizer::csv_tk { "\\", ",", "'\"{[", "'\"}]" };
Tokenizer Tokenizer::ssv_tk { "\\", " ", "'\"{[", "'\"}]" };

}

