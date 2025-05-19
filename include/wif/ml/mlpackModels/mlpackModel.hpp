/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Abstract Mlpack model interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

#include <memory>
#include <mlpack/core.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF::MlpackModels {

/**
 * @brief Abstract class which represents Mlpack model
 */
class MlpackModel {
public:
	/**
	 * @brief Construct a new model object with no loaded model
	 */
	MlpackModel() = default;

	/**
	 * @brief Destroy the MlpackModel object
	 *
	 */
	virtual ~MlpackModel() = default;

	/**
	 * @brief Classify single flowFeature object
	 * See std::vector<ClfResult> classify(const std::vector<FlowFeatures>&) for more details
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification
	 */
	virtual ClfResult classify(const FlowFeatures& flowFeatures) = 0;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> results of the classification
	 */
	virtual std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFeatures) = 0;

	/**
	 * @brief Load Mlpack model from file
	 *
	 * @param modelPath contains path to model file.
	 * @param logicalName contains the logical name of the trained model.
	 * @return Bool value true, if model was successfully loaded. False if not.
	 */
	virtual bool load(const std::string& modelPath, const std::string& logicalName = "trained_data")
		= 0;

	/**
	 * @brief Save Mlpack model to file
	 *
	 * @param modelPath contains path, where the model will be saved.
	 * @param logicalName contains the logical name of the trained model.
	 * @return Bool value true, if model was successfully saved. False if not.
	 */
	virtual bool
	save(const std::string& modelPath, const std::string& logicalName = "trained_data") const
		= 0;

	/**
	 * @brief Return information about if ML model is loaded or not
	 *
	 * @return bool true, if ML model is loaded. False, if not.
	 */
	bool isLoaded() const;

	/**
	 * @brief Getter for path of the used ML model
	 * @return const std::string&
	 */
	std::string getPath() const;

	/**
	 * @brief Set feature IDs which will be used for classification
	 *
	 * @param sourceFeatureIDs
	 */
	void setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs);

protected:
	/**
	 * @brief Convert data from FlowFeatures type to arma::vec type
	 * @param flowFeatures reference contains data in FlowFeatures type, which will be converted.
	 * @param vect reference contains initialized vector in arma::vec type, where flowFeatures will
	 * be written. vect must have same dimension as flowFeatures.
	 */
	void convertFeaturesToVector(const FlowFeatures& flowFeatures, arma::vec& vect) const;

	/**
	 * @brief Convert data from vector<FlowFeatures> type to arma::mat type
	 * @param burstOfFeatures reference contains data in vector<FlowFeatures> type, which will be
	 * converted.
	 * @param matrix reference contains initialized matrix in arma::mat type, where burstOfFeatures
	 * will be written. matrix must have same dimension as burstOfFeatures.
	 */
	void convertBurstOfFeaturesToMatrix(
		const std::vector<FlowFeatures>& burstOfFeatures,
		arma::mat& matrix) const;

	/**
	 * @brief The path to currently loaded ML model path
	 */
	std::string m_modelPath;

	/**
	 * @brief Bool value is true, if any ML model is correctly loaded. Otherwise it contains false
	 */
	bool m_loaded = false;

	/**
	 * @brief Vector of feature IDs, which were set in setFeatureIDs method
	 */
	std::vector<FeatureID> m_featureIDs;
};

} // namespace WIF::MlpackModels
