/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Decision tree model interface
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
#include <mlpack/methods/decision_tree/decision_tree.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF::MlpackModels {

/**
 * @brief Class which provides Decision tree model from Mlpack library
 */
class DecisionTreeModel : public MlpackModel {
public:
	/**
	 * @brief Construct a new Decision tree object with no loaded model
	 */
	DecisionTreeModel() = default;

	/**
	 * @brief Construct a new Decision tree object
	 *
	 * @param modelPath contains path to trained model file
	 */
	DecisionTreeModel(const std::string& modelPath);

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
	 * @brief Load Decision tree model from file
	 *
	 * @param modelPath contains path to model file.
	 * @return Bool value true, if model was successfully loaded. False if not.
	 */
	bool load(const std::string& modelPath) override;

	/**
	 * @brief Save Decision tree model to file
	 *
	 * @param modelPath contains path, where the model will be saved.
	 * @return Bool value true, if model was successfully saved. False if not.
	 */
	bool save(const std::string& modelPath) const override;

	/**
	 * @brief Train Decision tree model
	 *
	 * @param data contains training vector of flow features.
	 * @param labels contains training labels, between 0 and numClasses - 1 (inclusive). Should have
	 * length data.length().
	 * @param path contains path, where file will be saved.
	 * @param numClasses contains number of classes in the dataset.
	 * @param minLeafSize contains minimum number of points in each leaf node.
	 * @param minGainSplit contains minimum gain for a node to split.
	 * @param maxDepth contains maximum depth for the tree. (0 means no limit.)
	 */
	void train(
		std::vector<FlowFeatures>& data,
		std::vector<size_t>& labels,
		const std::string& path,
		size_t numClasses = 2,
		size_t minLeafSize = 10,
		double minGainSplit = 1e-7,
		size_t maxDepth = 0);

private:
	/**
	 * @brief Decision tree model
	 */
	mlpack::DecisionTree<> m_dt;
};

} // namespace WIF::MlpackModels
