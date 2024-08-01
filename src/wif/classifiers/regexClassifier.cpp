/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Regex classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/regexClassifier.hpp"

namespace WIF {

RegexClassifier::RegexClassifier(
	const RegexPattern& regexPattern,
	std::unique_ptr<Combinator> combinator)
	: m_regexPattern(regexPattern)
	, m_combinator(std::move(combinator))
{
	validateCombinator();
}

ClfResult RegexClassifier::classify(const FlowFeatures& flowFeatures)
{
	std::vector<double> flowFeatureResults;
	for (FeatureID featureID : getSourceFeatureIDs()) {
		double matchResult = matchPattern(flowFeatures.get<std::string>(featureID));
		flowFeatureResults.emplace_back(matchResult);
	}

	return ClfResult(m_combinator->combine(flowFeatureResults));
}

std::vector<ClfResult>
RegexClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures)
{
	std::vector<ClfResult> burstResults;
	burstResults.reserve(burstOfFlowsFeatures.size());

	for (const auto& flowFeatures : burstOfFlowsFeatures) {
		burstResults.emplace_back(classify(flowFeatures));
	}

	return burstResults;
}

void RegexClassifier::validateCombinator() const
{
	if (m_combinator == nullptr) {
		throw std::runtime_error(
			"RegexClassifier::validateCombinator() has failed. Combinator is not set!");
	}
}

double RegexClassifier::matchPattern(const std::string& str) const
{
	return m_regexPattern.match(str);
}

} // namespace WIF
