/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Regex classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/classifier.hpp"
#include "wif/combinators/combinator.hpp"
#include "wif/regex/regexPattern.hpp"

#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Classifier performing pattern-matching based detection
 */
class RegexClassifier : public Classifier {
public:
	/**
	 * @brief Construct a new Regex Classifier object
	 *
	 * @throw std::runtime_error if combinator is not set
	 *
	 * @param regexPattern regex pattern to match in each of the set feature IDs
	 * @param combinator used to combine subresults from all features to produce the single result
	 * for each flow
	 */
	RegexClassifier(const RegexPattern& regexPattern, std::unique_ptr<Combinator> combinator);

	/**
	 * @brief Classify single flowFeature object
	 * Source fields of flowFeatures objects must hold a value of type std::string
	 * If more source fields is set, the sub-results are combined together by a Combinator obtained
	 * in the constructor
	 *
	 * ClfResult contains a single double value obtained from the combinator
	 *
	 * @param flowFeatures
	 * @return ClfResult
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures
	 * @return std::vector<ClfResult>
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures) override;

private:
	double matchPattern(const std::string& str) const;
	void validateCombinator() const;

	RegexPattern m_regexPattern;
	std::unique_ptr<Combinator> m_combinator;
};

} // namespace WIF
