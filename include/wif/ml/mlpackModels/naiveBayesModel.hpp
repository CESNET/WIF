/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Naive Bayes model interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/ml/mlpackModels/mlpackModel.hpp"
#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

#include <armadillo>
#include <memory>
#include <mlpack/core.hpp>
#include <mlpack/methods/naive_bayes/naive_bayes_classifier.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF::MlpackModels {

/**
 * @brief Class which provides Naive Bayes model from Mlpack library
 */
class NaiveBayesModel : public MlpackModel {
public:
	/**
	 * @brief Construct a new Naive Bayes object with no loaded model
	 */
	NaiveBayesModel() = default;

	/**
	 * @brief Construct a new Naive Bayes object
	 *
	 * @param modelPath contains path to trained model file.
	 */
	NaiveBayesModel(const std::string& modelPath);

	/**
	 * @brief Classify single flowFeature object
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification, which contains vector<double> with
	 * probabilities for each class.
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> the results of the classification. Each ClfResult contains
	 * result of the classification, which contains vector<double> with probabilities for each
	 * class.
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFeatures) override;

	/**
	 * @brief Load Naive Bayes model from file
	 *
	 * @param modelPath contains path to model file.
	 * @return Bool value true, if model was successfully loaded. False if not.
	 */
	bool load(const std::string& modelPath) override;

	/**
	 * @brief Save Naive Bayes model to file
	 *
	 * @param modelPath contains path, where the model will be saved.
	 * @return Bool value true, if model was successfully saved. False if not.
	 */
	bool save(const std::string& modelPath) const override;

	/**
	 * @brief Train Naive Bayes model
	 *
	 * @param data contains training vector of flow features.
	 * @param labels contains training labels, between 0 and numClasses - 1 (inclusive). Should have
	 * length data.length().
	 * @param path contains path, where file will be saved.
	 * @param numClasses contains number of classes in the dataset.
	 */
	void train(
		const std::vector<FlowFeatures>& data,
		const std::vector<size_t>& labels,
		const std::string& path,
		size_t numClasses = 2);

private:
	/**
	 * @brief Naive Bayes model
	 */
	mlpack::NaiveBayesClassifier<> m_nbc;
};

} // namespace WIF::MlpackModels
