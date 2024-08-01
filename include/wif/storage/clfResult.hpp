/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Clf result interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/utils/typeTraits.hpp"
#include <variant>
#include <vector>

namespace WIF {

/**
 * @brief ClfResult class
 *
 * Represents a result of a classifier's classify() method
 */
class ClfResult {
	template<typename... Types>
	struct VariantData {
		using AllowedTypesList = TypeList<Types...>;
		using VariantType = std::variant<Types...>;
	};

	using ClfVariant = VariantData<double, std::vector<double>>;

	using AllowedVariantTypes = ClfVariant::AllowedTypesList;
	using VariantType = ClfVariant::VariantType;

public:
	/**
	 * @brief Construct a new Clf Result objet
	 *
	 * @param featuresCount object reserves memory for provided feature count
	 */
	template<typename T, typename = is_type_allowed<T, AllowedVariantTypes>()>
	ClfResult(const T& value)
	{
		m_result = value;
	}

	/**
	 * @brief Get result as type T
	 *
	 * @return T
	 */
	template<typename T, typename = is_type_allowed<T, AllowedVariantTypes>()>
	const T& get() const
	{
		return std::get<T>(m_result);
	}

private:
	VariantType m_result;
};

} // namespace WIF
