#pragma once

/**
 * Set of implementations of template function print_ip, developed for outputting IP addresses, represented by different types.
 */

#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <tuple>

 /**
  * Outputs an ip address represented as an integral type.
  */
template<typename T>
std::enable_if_t<std::is_integral_v<T>, void>
print_ip(T const& value)
{
	size_t constexpr octets_number = sizeof(T);

	for (size_t i = octets_number; i--;)
	{
		std::cout << (value >> (i << 3) & 0xFF);
		if (i > 0) {
			std::cout << '.';
		}
	}

	std::cout << std::endl;
}

/**
 * Outputs an ip address represented as std::string.
 */
template<typename STR,
	typename = std::enable_if_t<std::is_convertible_v<STR, std::string>>>
void print_ip(STR const & value )
{
	std::cout << value << std::endl;
}

/**
 * Outputs an ip address represented as std::vector or std::list.
 */
template <
	template <typename, typename> typename Container,
	typename Type,
	typename Allocator = std::allocator<Type>,
	typename = std::enable_if_t<
	std::is_same_v<Container<Type, Allocator>, std::vector<Type, Allocator>> ||
	std::is_same_v<Container<Type, Allocator>, std::list<Type, Allocator>>>>
void print_ip(const Container<Type, Allocator>& container)
{
	for (auto iter = std::begin(container); iter != std::end(container); ++iter) {
		if (iter != std::begin(container)) { std::cout << ", "; }
			
		std::cout << *iter;
	}

	std::cout << std::endl;
}

/**
 * Special structure used to check and output a value from std::tuple.
 */
template <size_t NUM>
struct PrintTupleIP
{
	template <typename Type0, typename... Types>
	static constexpr void Print(const std::tuple<Type0, Types...>& tuple)
	{
		auto tuple_value = std::get<sizeof...(Types) + 1 - NUM>(tuple);
		static_assert (std::is_same_v<Type0, decltype(tuple_value)>, "Tuple must contain only the same types!");

		std::cout << tuple_value;
		if constexpr (NUM > 1) { std::cout << '.'; }

		PrintTupleIP<NUM - 1>::Print(tuple);
	}
};

/**
 * Special structure used to finish tuple output.
 */
template <>
struct PrintTupleIP<0>
{
	template <typename... Types>
	static constexpr void Print(const std::tuple<Types...>&) {}
};

template <typename>
struct is_tuple : std::false_type {};

template <typename ...Types>
struct is_tuple<std::tuple<Types...>> : std::true_type {};

/**
 * Outputs an ip address represented as std::tuple filled in with the same types.
 */
template <typename ...Types>
void print_ip(const std::tuple<Types...>& tuple)
{
	PrintTupleIP<sizeof...(Types)>::Print(tuple);
	std::cout << std::endl;
}