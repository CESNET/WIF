/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Flow features interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/dataVariant.hpp"

#include <cstdint>
#include <string>

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
	 * @brief Get raw feature
	 *
	 * @param featureID the feature identifier
	 * @return const DataVariant& raw feature
	 */
	const DataVariant& getRaw(FeatureID featureID) const { return m_features.at(featureID); }

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
	std::vector<DataVariant> m_features;
};

} // namespace WIF
