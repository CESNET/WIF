/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn ML classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/classifier.hpp"
#include "wif/flowFeatures.hpp"
#include "wif/ml/scikitlearnWrapper.hpp"

#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Classifier performing Machine-Learning based detection via Scikit-learn library
 */
class ScikitlearnMlClassifier : public Classifier {
	using MlFeatures = ScikitlearnWrapper::MlFeatures;

public:
	/**
	 * @brief Construct a new Scikitlearn Ml Classifier object
	 *
	 * @throw std::runtime_error if Python's bridge fails to init properly
	 */
	ScikitlearnMlClassifier(
		const std::string& bridgePath,
		const std::string& mlModelPath,
		size_t targetClassProbabilityIdx);

	/**
	 * @brief Classify single flowFeature object
	 *
	 * @param flowFeatures
	 * @return double
	 */
	double classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures
	 * @return std::vector<double>
	 */
	std::vector<double> classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures) override;

private:
	inline MlFeatures createFeaturesFromFlow(const FlowFeatures& flowFeatures) const;

	std::unique_ptr<ScikitlearnWrapper> m_scikitlearnWrapper;
};

} // namespace WIF
