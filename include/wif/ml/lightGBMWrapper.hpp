/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief LightGBM wrapper interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

#include <LightGBM/c_api.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF {

/**
 * @brief Wrapper class which provides a bridge to LightGBM library
 */
class LightGBMWrapper {
public:
	/**
	 * @brief Construct a new LightGBM wrapper object
	 */
	LightGBMWrapper();

	/**
	 * @brief Construct a new LightGBM wrapper object
	 *
	 * @param modelPath contains path to the model file
	 */
	LightGBMWrapper(const std::string& modelPath);

	/**
	 * @brief Destruct the LightGBM wrapper object
	 */
	~LightGBMWrapper();

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
	const std::string& getModelPath() const;

	/**
	 * @brief Load the model from the file
	 *
	 * @param modelPath contains path to the model file.
	 * @return bool true, if model was succesfully loaded. False if not.
	 */
	bool loadModel(const std::string& modelPath);

	/**
	 * @brief Classify single flowFeature object
	 * See std::vector<ClfResult> classify(const std::vector<FlowFeatures>&) for more details
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification, which contains
	 * vector<double> with probabilities for each class
	 */
	ClfResult classify(const FlowFeatures& flowFeatures);

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> the results of the classification. Each ClfResult contains
	 * result of the classification, which contains vector<double> with
	 * probabilities for each class
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFeatures);

	/**
	 * @brief Return information about if ML model is loaded or not
	 *
	 * @return bool true, if ML model is loaded. False, if not
	 */
	bool isLoaded() const;

	/**
	 * @brief Train LightGBM model
	 *
	 * @param datasetFileName is the name of the file with training data
	 * @param datasetParams are the additional parameters of the training dataset
	 * @param numOfIterations is the number of training iterations (how many
	 * LGBM_BoosterUpdateOneIter function is called, see
	 * https://lightgbm.readthedocs.io/en/stable/C-API.html)
	 * @param params are parameters in format ‘key1=value1 key2=value2’ (see
	 * https://lightgbm.readthedocs.io/en/stable/C-API.html)
	 * @param modelFileName name of the file where the trained model will be saved
	 */
	void train(
		const std::string& datasetFileName,
		const char* datasetParams = "header=true label=name:label",
		const unsigned numOfIterations = 100,
		const char* params
		= "boosting_type=gbdt objective=binary metric=auc num_leaves=30 learning_rate=0.05 "
		  "feature_fraction=0.9",
		const std::string modelFileName = "model.txt");

private:
	/**
	 * @brief LightGBM model
	 */
	BoosterHandle m_booster = nullptr;

	/**
	 * @brief Number of iterations of m_booster
	 */
	int m_outNumIterations = 0;

	/**
	 * @brief Bool value is true, if any ML model is correctly loaded. Otherwise it contains false
	 */
	bool m_isLoaded = false;

	/**
	 * @brief The path to currently loaded ML model path
	 */
	std::string m_modelPath;

	/**
	 * @brief Vector of feature IDs, which were set in setFeatureIDs method
	 */
	std::vector<FeatureID> m_featureIDs;
};

} // namespace WIF
