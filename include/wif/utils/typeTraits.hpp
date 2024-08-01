/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Type traits
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <tuple>
#include <type_traits>

namespace WIF {

// NOLINTBEGIN

template<typename Needle>
constexpr bool is_type_in_list()
{
	return false;
}

template<typename Needle, typename Haystack0, typename... Haystack>
constexpr bool is_type_in_list()
{
	if constexpr (std::is_same<Needle, Haystack0>::value) {
		return true;
	} else {
		return is_type_in_list<Needle, Haystack...>();
	}
}

template<typename Needle, typename... Haystack>
struct is_type_allowed : std::enable_if_t<is_type_in_list<Needle, Haystack...>()> {};

template<typename... Types>
struct TypeList {
	template<std::size_t N>
	using type = typename std::tuple_element<N, std::tuple<Types...>>::type;
};

template<std::size_t index, typename TypeList>
using TypeListElement = typename TypeList::template type<index>;

// NOLINTEND

} // namespace WIF
