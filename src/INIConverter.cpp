#include "INI/INIConverter.h"

#include <cstdlib>
#include <stack>

#include <boost/tokenizer.hpp>

using namespace std;

namespace engine2
{

namespace ini
{

const string ESCAPE_LIST { R"_("'{}[])_" };

string escape(const string&);
string unescape(const string&);

/*
 * toString
 */

template<>
string INIConverter<int>::toString(const type &value)
{
	return to_string(value);
}

template<>
string INIConverter<bool>::toString(const type &value)
{
	return (value ? "true" : "false");
}

template<>
string INIConverter<float>::toString(const type &value)
{
	return to_string(value);
}

template<>
string INIConverter<string>::toString(const type &value)
{
	return "\"" + escape(value) + "\"";
}

/*
 * toValue
 */

template<>
auto INIConverter<int>::toValue(const string &str) -> type
{
	return atoi(str.c_str());
}

template<>
auto INIConverter<bool>::toValue(const string &str) -> type
{
	string copy = boost::trim_copy(str);
	boost::to_lower(copy);
	return copy.substr(0, 4) == "true";
}

template<>
auto INIConverter<float>::toValue(const string &str) -> type
{
	return atof(str.c_str());
}

template<>
auto INIConverter<string>::toValue(const string &str) -> type
{
	size_t start = str.find('"');
	size_t end = str.rfind('"');
	
	// start can't be npos without end being npos
	if (   end == string::npos
	    || end == start) {
		return "";
	}
	
	start++;
	
	string sub = str.substr(start, end - start);
	
	return unescape(sub);
}

string escape(const string &str)
{
	string result;
	
	for (char c : str) {
		if (ESCAPE_LIST.find(c) != string::npos) {
			result += '\\';
		}
		
		result += c;
	}
	
	return result;
}

string unescape(const string &str)
{
	string result;
	
	for (auto it = str.begin(); it != str.end(); ++it) {
		if ('\\' == *it) {
			++it;
		}
		
		result += *it;
	}
	
	return result;
}

} // namespace ini

} // namespace engine2

