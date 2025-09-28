/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/genericMlClassifier.hpp"
#include "wif/ml/mlpackWrapper.hpp"

#include <memory>
#include <string>
#include <vector>

namespace WIF {

/**
 * @brief Classifier performing ML classification which is interconnected with Mlpack library
 *
 */
class MlpackClassifier : public GenericMlClassifier {
public:
	/**
	 * @brief Construct a new Mlpack Classifier object
	 *
	 * @param path contains the path to the file with the trained model.
	 * @param logicalName contains the logical name of the trained model.
	 */
	MlpackClassifier(const std::string& path, const std::string& logicalName = "trained_data");

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
	 * @return ClfResult result of the classification, which contains double represention class or
	 * vector<double> with probabilities for each class (depends on model).
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
	 * @brief Reload the model from file, which was set in the constructor
	 *
	 * @param logicalName contains the logical name of the trained model
	 */
	void reloadModelFromDisk(const std::string& logicalName = "trained_data") override;

private:
	/**
	 * @brief Pointer to wrapper object with loaded mlpack model
	 */
	std::unique_ptr<MlpackWrapper> m_mlpackWrapper;

	mutable std::string m_modelPath;
};

} // namespace WIF
