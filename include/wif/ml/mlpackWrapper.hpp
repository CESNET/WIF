/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack wrapper interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "mlpackModels/adaboostModel.hpp"
#include "mlpackModels/decisionTreeModel.hpp"
#include "mlpackModels/hoeffdingTreeModel.hpp"
#include "mlpackModels/linearSVMModel.hpp"
#include "mlpackModels/mlpackModel.hpp"
#include "mlpackModels/naiveBayesModel.hpp"
#include "mlpackModels/perceptronModel.hpp"
#include "mlpackModels/randomForestModel.hpp"
#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

#include <armadillo>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <mlpack/core.hpp>
#include <mlpack/methods/decision_tree/decision_tree.hpp>
#include <mlpack/methods/perceptron/perceptron.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF {

/**
 * @brief Wrapper class which provides a bridge to Mlpack library
 */
class MlpackWrapper {
public:
	/**
	 * @brief Construct a new Mlpack wrapper object
	 *
	 * @param modelPath contains path to the model file.
	 * @param logicalName contains the logical name of the trained model.
	 */
	MlpackWrapper(const std::string& modelPath, const std::string& logicalName = "trained_data");

	/**
	 * @brief Set feature IDs which will be used for classification
	 *
	 * @param sourceFeatureIDs
	 */
	void setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs);

	/**
	 * @brief Getter for path of the used ML model
	 * @return const std::string&
	 */
	const std::string getModelPath() const;

	/**
	 * @brief Load the model from the file
	 *
	 * @param modelPath contains path to the model file.
	 * @param logicalName contains the logical name of the trained model.
	 * @return bool true, if model was succesfully loaded. False if not.
	 */
	bool loadModel(const std::string& modelPath, const std::string& logicalName = "trained_data");

	/**
	 * @brief Classify single flowFeature object
	 * See std::vector<ClfResult> classify(const std::vector<FlowFeatures>&) for more details
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification, which contains double representing class or
	 * vector<double> with probabilities for each class (depends on model).
	 */
	ClfResult classify(const FlowFeatures& flowFeatures);

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> the results of the classification. Each ClfResult contains
	 * result of the classification, which contains double represention class or vector<double> with
	 * probabilities for each class (depends on model).
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFeatures);

	/**
	 * @brief Return information about if ML model is loaded or not
	 *
	 * @return bool true, if ML model is loaded. False, if not.
	 */
	bool isLoaded() const;

private:
	/**
	 * @brief Method to initialize model file prefixes
	 */
	void initPrefixes();

	/**
	 * @brief Return name of the model according to the prefix
	 */
	std::string getModelNameFromPrefix(const std::string& modelPath);

	/**
	 * @brief Pointer to ML model
	 */
	std::unique_ptr<WIF::MlpackModels::MlpackModel> m_model;

	/**
	 * @brief Vector of feature IDs, which were set in setFeatureIDs method
	 */
	std::vector<FeatureID> m_featuresIDs;

	/**
	 * @brief Map of filename prefixes assigned to specific ML model types
	 */
	std::map<std::string, std::string> m_prefixes;
};

} // namespace WIF
