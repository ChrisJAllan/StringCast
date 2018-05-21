#ifndef INICONVERTER_H
#define INICONVERTER_H

#include <string>

#include <boost/tokenizer.hpp>

#include "Constraints.h"
#include "Util/Tokenizer.h"

namespace engine2
{

namespace ini
{

using namespace std;

/**
 * @brief Class to handle conversions to and from INI value types.
 */
template<class T>
class INIConverter
{
public:
	typedef INI_TYPE(T) type;
	
	static string toString(const type&);
	static type toValue(const string&);
};

/**
 * @brief Vector specialization
 */
template<class T>
class INIConverter<vector<T>>
{
public:
	typedef INI_TYPE(vector<T>) type;
	
	static string toString(const type&);
	static type toValue(const string&);
};

// Implementation

template<class T> auto INIConverter<vector<T>>::toString(const type& values) -> string
{
	string text = "[";
	for (T value : values) {
		text += INIConverter<T>::toString(value) + ", ";
	}
	if (values.size() > 0) {
		text.pop_back();
		text.pop_back();
	}
	text += "]";
	return text;
}
template<class T> auto INIConverter<vector<T>>::toValue(const string &str) -> type
{
	vector<T> values;
	string copy = str.substr(1, str.size() - 2);
	
	boost::tokenizer<Tokenizer> tok(copy, Tokenizer::csv_tk);
	
	for (auto it = tok.begin(); it != tok.end(); ++it) {
		values.push_back(INIConverter<T>::toValue(*it));
	}
	
	return values;
}

/**
 * @brief Tuple specialization
 */
template<class... Ts>
class INIConverter<tuple<Ts...>>
{
public:
	typedef INI_TYPE(tuple<Ts...>) type;
	
	static string toString(const type&);
	static type toValue(const string&);
};

// Helper functions

template<size_t I = 0, class... Ts>
/* string */ typename enable_if<I == sizeof...(Ts) - 1, string>::type
tuple_to_string(const tuple<Ts...> &tpl)
{
	auto value = get<I>(tpl);
	return INIConverter<decltype(value)>::toString(value);
}

template<size_t I = 0, class... Ts>
/* string */ typename enable_if<I < sizeof...(Ts) - 1, string>::type
tuple_to_string(const tuple<Ts...> &tpl)
{
	auto value = get<I>(tpl);
	return   INIConverter<decltype(value)>::toString(value)
	       + ", "
	       + tuple_to_string<I+1>(tpl);
}

template<size_t I = 0, class Iterator, class... Ts>
/* void */ typename enable_if<I == sizeof...(Ts) - 1>::type
string_to_tuple(Iterator &it, const Iterator &end, tuple<Ts...> &tpl)
{
	typedef typename std::tuple_element_t<I, tuple<Ts...>> type;
	
	if (it != end) {
		get<I>(tpl) = INIConverter<type>::toValue(*it);
	}
	else {
		get<I>(tpl) = INIConverter<type>::toValue("");
	}
}

template<size_t I = 0, class Iterator, class... Ts>
/* void */ typename enable_if<I < sizeof...(Ts) - 1>::type
string_to_tuple(Iterator &it, const Iterator &end, tuple<Ts...> &tpl)
{
	typedef typename std::tuple_element_t<I, tuple<Ts...>> type;
	
	if (it != end) {
		get<I>(tpl) = INIConverter<type>::toValue(*it);
		string_to_tuple<I + 1>(++it, end, tpl);
	}
	else {
		get<I>(tpl) = INIConverter<type>::toValue("");
		string_to_tuple<I + 1>(it, end, tpl);
	}
}

// Implementation

template<class... Ts> auto INIConverter<tuple<Ts...>>::toString(const type &value) -> string
{
	return "{" + tuple_to_string(value) + "}";
}
template<class... Ts> auto INIConverter<tuple<Ts...>>::toValue(const string &str) -> type
{
	string copy = str.substr(1, str.size() - 2);
	boost::tokenizer<Tokenizer> tok(copy, Tokenizer::csv_tk);
	
	type value;
	auto it = tok.begin();
	
	string_to_tuple(it, tok.end(), value);
	return value;
}

}

}

#endif // INICONVERTER_H
