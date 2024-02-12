/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn ML classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/classifier.hpp"
#include "wif/ml/scikitMlWrapper.hpp"

#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Classifier performing Machine-Learning based detection via Scikit-learn library
 */
class ScikitMlClassifier : public Classifier {
public:
	/**
	 * @brief Construct a new Scikitlearn Ml Classifier object
	 *
	 * @param bridgePath path to Python script
	 * @param mlModelPath path to .pickle file with trained Machine Learning model
	 */
	ScikitMlClassifier(const std::string& bridgePath, const std::string& mlModelPath);

	/**
	 * @brief Get the path to the used ML model
	 *
	 * @return const std::string& the path to the used ML model
	 */
	const std::string& getMlModelPath() const noexcept;

	/**
	 * @brief Set feature IDs which will be used for classification
	 *
	 * @param sourceFeatureIDs
	 */
	void setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs) override;

	/**
	 * @brief Reload used ML model from disk
	 */
	void reloadModelFromDisk();

	/**
	 * @brief Classify single flowFeature object
	 *
	 * @param flowFeatures
	 * @return double
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures
	 * @return std::vector<double>
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures) override;

private:
	std::unique_ptr<ScikitMlWrapper> m_scikitMlWrapper;
};

} // namespace WIF
