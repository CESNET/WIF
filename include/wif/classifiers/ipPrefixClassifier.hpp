/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief IP prefix classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/classifier.hpp"
#include "wif/utils/ipPrefix.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Classifier performing blocklist detection
 */
class IpPrefixClassifier : public Classifier {
public:
	/**
	 * @brief Construct an IP Prefix Classifier object with empty blocklist
	 */
	IpPrefixClassifier() = default;

	/**
	 * @brief Construct a new IP Prefix Classifier object
	 * @param blocklist std::vector of blocklisted IP prefixes
	 */
	IpPrefixClassifier(const std::vector<IpPrefix>& blocklist);

	/**
	 * @brief Getter for current IP prefix blocklist
	 * @return const std::vector<IpPrefix>& current blocklist
	 */
	const std::vector<IpPrefix>& getBlocklist() const noexcept { return m_blocklist; }

	/**
	 * @brief Update blocklist
	 * Old blocklist is destructed and replaced by new one
	 * @param blocklist std::vector of new IP prefixes on blocklist
	 */
	void updateBlocklist(const std::vector<IpPrefix>& blocklist);

	/**
	 * @brief Classify single flowFeature object
	 * Source fields of flowFeatures objects must hold a value of type WIF::IpAddress
	 *
	 * ClfResult contains a single non-zero double value if flowFeatures contained a field with IP
	 * from blocklist, otherwise the double value is set to zero
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> std::vector<ClfResult> the results of the classification
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures) override;

private:
	bool findIpAddress(const IpAddress& ipAddress);

	std::vector<IpPrefix> m_blocklist;
};

} // namespace WIF
