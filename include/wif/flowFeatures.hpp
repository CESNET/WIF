/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Flow features interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <string>
#include <variant>
#include <vector>

namespace WIF {

using FeatureID = uint16_t;

/**
 * @brief FlowFeatures class
 *
 * Represents a flow in memory
 */
class FlowFeatures {
public:
	/**
	 * @brief Construct a new Flow Features object
	 *
	 * @param featuresCount object reserves memory for provided feature count
	 */
	FlowFeatures(size_t featuresCount);

	/**
	 * @brief Get feature
	 *
	 * @tparam T feature type
	 * @param featureID
	 * @return T feature
	 */
	template<typename T>
	const T& get(FeatureID featureID) const
	{
		return std::get<T>(m_features.at(featureID));
	}

	/**
	 * @brief Set feature
	 *
	 * @tparam T feature type
	 * @param featureID
	 * @param newValue
	 */
	template<typename T>
	void set(FeatureID featureID, T newValue)
	{
		m_features[featureID] = newValue;
	}

	/**
	 * @brief Flow size
	 *
	 * @return size_t number of features
	 */
	size_t size() const;

	/**
	 * @brief Clear object to the initial state
	 */
	void clear();

private:
	using SupportedVariantTypes
		= std::variant<std::monostate, uint8_t, uint16_t, uint32_t, uint64_t, double, std::string>;
	std::vector<SupportedVariantTypes> m_features;
};

} // namespace WIF
