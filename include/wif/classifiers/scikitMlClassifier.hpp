/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Scikit-learn ML classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/genericMlClassifier.hpp"
#include "wif/ml/scikitMlWrapper.hpp"

#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Classifier performing Machine-Learning based detection via Scikit-learn library
 */
class ScikitMlClassifier : public GenericMlClassifier {
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
	const std::string& getMlModelPath() const noexcept override;

	/**
	 * @brief Set feature IDs which will be used for classification
	 *
	 * @param sourceFeatureIDs
	 */
	void setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs) override;

	/**
	 * @brief Reload used ML model from disk
	 *
	 * @param logicalName is unused
	 */
	void reloadModelFromDisk([[maybe_unused]] const std::string& logicalName) override;

	/**
	 * @brief Classify single flowFeature object
	 * Source fields of flowFeatures objects must hold a value of type double
	 * Fields are passed to the ML model in the order of passed source feature IDs
	 *
	 * ClfResult contains std::vector<double> representing output of predict_proba() call
	 * with probabilities of each class
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
	std::unique_ptr<ScikitMlWrapper> m_scikitMlWrapper;
};

} // namespace WIF
