#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace engine2
{

namespace constraints
{

using namespace std;

/**
 * INI value constraints.
 * 
 * is_ini_type<T> is either std::true_type or std::false_type.
 * 
 * INI_TYPE(T) evaluates to T if T is a valid INI value type.
 */

#define INI_TYPE(T) typename std::enable_if<engine2::constraints::is_ini_type<T>::value, T>::type

template<class T>
struct is_ini_type : public false_type { };

template<> struct is_ini_type<int> : public true_type { };
template<> struct is_ini_type<bool> : public true_type { };
template<> struct is_ini_type<float> : public true_type { };
template<> struct is_ini_type<string> : public true_type { };

template<class T>
struct is_ini_type<tuple<T>> : public is_ini_type<T> { };

template<class T, class... Ts>
struct is_ini_type<tuple<T, Ts...>>
{
	const static bool value = is_ini_type<T>::value && is_ini_type<tuple<Ts...>>::value;
};

template<class T>
struct is_ini_type<vector<T>> : public is_ini_type<T> { };

/**
 * Hashable type constraint.
 * 
 * is_hashable<T> is either std::true_type or std::false_type.
 */

inline namespace helpers {
template<class T> true_type  hashable_helper(const T&, const typename hash<T>::result_type* = nullptr);
template<class T> false_type hashable_helper(...);
}

template<typename T>
struct is_hashable : decltype(hashable_helper<T>(declval<T>())) { };

/**
 * Less-than comparable constraint.
 * 
 * is_lt_comparable<T> is either std::true_type or std::false_type.
 */

inline namespace helpers {
template<class T> true_type  lt_comp_helper(const T&, decltype(declval<T>() < declval<T>())* = nullptr);
template<class T> false_type lt_comp_helper(...);
}

template<class T>
struct is_lt_comparable : decltype(lt_comp_helper<T>(declval<T>())) { };

/**
 * Equal comparable constraint.
 * 
 * is_eq_comparable<T> is either std::true_type or std::false_type.
 */

inline namespace {
template<class T> true_type  eq_comparable(const T&, decltype(declval<T>() == declval<T>())* = nullptr);
template<class T> false_type eq_comparable(...);
}

template<class T>
using is_eq_comparable = decltype(eq_comparable<T>(declval<T>()));

} // namespace constraints

} // namespace engine2

#endif // CONSTRAINTS_H
