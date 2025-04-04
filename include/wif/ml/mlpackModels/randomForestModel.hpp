/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Random forest model interface
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
#include <mlpack/methods/random_forest/random_forest.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF::MlpackModels {

/**
 * @brief Class which provides Random forest model from Mlpack library
 */
class RandomForestModel : public MlpackModel {
public:
	/**
	 * @brief Construct a new Random forest object with no loaded model
	 */
	RandomForestModel() = default;

	/**
	 * @brief Construct a new Random forest object
	 *
	 * @param modelPath contains path to trained model file.
	 */
	RandomForestModel(const std::string& modelPath);

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
	 * @brief Load Random forest model from file
	 *
	 * @param modelPath contains path to trained model file.
	 * @return Bool value true, if model was successfully loaded. False if not.
	 */
	bool load(const std::string& modelPath) override;

	/**
	 * @brief Save Random forest model to file
	 *
	 * @param modelPath contains path, where the model will be saved.
	 * @return Bool value true, if model was successfully saved. False if not.
	 */
	bool save(const std::string& modelPath) const override;

	/**
	 * @brief Train Random forest model
	 *
	 * @param data contains training vector of flow features.
	 * @param labels contains training labels, between 0 and numClasses - 1 (inclusive). Should have
	 * length data.length().
	 * @param path contains path, where file will be saved.
	 * @param numClasses contains number of classes in the dataset.
	 * @param numTrees contains number of trees to train in the random forest.
	 * @param minLeafSize contains minimum number of points in each leaf node of each decision tree.
	 * @param minGainSplit contains minimum gain for a node to split in each decision tree.
	 * @param maxDepth contains maximum depth for each decision tree. (0 means no limit.)
	 * @param warmStart - if true, training adds numTrees trees to the random forest. If false, an
	 * entirely new random forest will be created.
	 */
	void train(
		const std::vector<FlowFeatures>& data,
		const std::vector<size_t>& labels,
		const std::string& path,
		size_t numClasses = 2,
		size_t numTrees = 20,
		size_t minLeafSize = 1,
		double minGainSplit = 1e-7,
		size_t maxDepth = 0,
		bool warmStart = false);

private:
	/**
	 * @brief Random forest model
	 */
	mlpack::RandomForest<> m_rf;
};

} // namespace WIF::MlpackModels
