/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief LightGBM classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/genericMlClassifier.hpp"
#include "wif/ml/lightGBMWrapper.hpp"

#include <memory>
#include <string>
#include <vector>

namespace WIF {
/**
 * @brief Classifier performing ML classification which is interconnected with LightGBM library
 *
 */
class LightGBMClassifier : public GenericMlClassifier {
public:
	/**
	 * @brief Construct a new LightGBM Classifier object
	 *
	 * @param path contains the path to the file with the trained model
	 */
	LightGBMClassifier(const std::string& path);

	/**
	 * @brief Set feature IDs which will be used for classification
	 *
	 * @param sourceFeatureIDs
	 */
	void setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs) override;

	/**
	 * @brief Classify single flowFeature object
	 * See std::vector<ClfResult> classify(const std::vector<FlowFeatures>&) for more details
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification, which contains
	 * vector<double> with probabilities for each class
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> classification results with ClfResult object for each flow
	 * features object
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFlowFeatures) override;

	/**
	 * @brief Return the path of the ML model, which is currently loaded
	 * @return const std::string& path of the model
	 */
	const std::string& getMlModelPath() const noexcept override;

	/**
	 * @brief Reload used ML model from disk
	 *
	 * @param logicalName is unused
	 */
	void
	reloadModelFromDisk([[maybe_unused]] const std::string& logicalName = "trained_data") override;

private:
	/**
	 * @brief Pointer to wrapper object with loaded lightGBM model
	 */
	std::unique_ptr<LightGBMWrapper> m_lightGBMWrapper;
};
} // namespace WIF
