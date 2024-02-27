/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief IP prefix classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/ipPrefixClassifier.hpp"

namespace WIF {

IpPrefixClassifier::IpPrefixClassifier(const std::vector<IpPrefix>& blocklist)
{
	updateBlocklist(blocklist);
}

void IpPrefixClassifier::updateBlocklist(const std::vector<IpPrefix>& blocklist)
{
	m_blocklist = blocklist;
	std::sort(m_blocklist.begin(), m_blocklist.end());
}

ClfResult IpPrefixClassifier::classify(const FlowFeatures& flowFeatures)
{
	for (FeatureID featureID : getSourceFeatureIDs()) {
		auto ipAddress = flowFeatures.get<IpAddress>(featureID);
		if (findIpAddress(ipAddress)) {
			return ClfResult(1.0);
		}
	}
	return ClfResult(0.0);
}

std::vector<ClfResult>
IpPrefixClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures)
{
	std::vector<ClfResult> burstResults;
	burstResults.reserve(burstOfFlowsFeatures.size());
	for (const auto& flowFeatures : burstOfFlowsFeatures) {
		burstResults.emplace_back(classify(flowFeatures));
	}
	return burstResults;
}

bool IpPrefixClassifier::findIpAddress(const IpAddress& ipAddress)
{
	return std::binary_search(m_blocklist.begin(), m_blocklist.end(), ipAddress);
}

} // namespace WIF
