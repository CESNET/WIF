/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Hoeffding tree model interface
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
#include <mlpack/methods/hoeffding_trees.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF::MlpackModels {

/**
 * @brief Class which provides Hoeffding tree model from Mlpack library
 */
class HoeffdingTreeModel : public MlpackModel {
public:
	/**
	 * @brief Construct a new Hoeffding tree object with no loaded model
	 */
	HoeffdingTreeModel() = default;

	/**
	 * @brief Construct a new Hoeffding tree object
	 *
	 * @param modelPath contains path to trained model file.
	 */
	HoeffdingTreeModel(const std::string& modelPath);

	/**
	 * @brief Classify single flowFeature object
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification, which contains double representing class.
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> the results of the classification. Each ClfResult contains
	 * result of the classification, which contains double representing class.
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFeatures) override;

	/**
	 * @brief Load Hoeffding tree model from file
	 *
	 * @param modelPath contains path to model file.
	 * @return Bool value true, if model was successfully loaded. False if not.
	 */
	bool load(const std::string& modelPath) override;

	/**
	 * @brief Save Hoeffding tree model to file
	 *
	 * @param modelPath contains path, where the model will be saved.
	 * @return Bool value true, if model was successfully saved. False if not.
	 */
	bool save(const std::string& modelPath) const override;

	/**
	 * @brief Train Hoeffding tree model
	 *
	 * @param data contains training vector of flow features.
	 * @param labels contains training labels, between 0 and numClasses - 1 (inclusive). Should have
	 * length data.length().
	 * @param path contains path, where file will be saved.
	 * @param numClasses contains number of classes in the dataset.
	 * @param batchTraining - if true, a batch training algorithm is used, instead of the usual
	 * incremental algorithm. This is generally more efficient for larger datasets.
	 * @param successProbability contains probability of success required for Hoeffding bound before
	 * a node split can happen.
	 * @param maxSamples contains maximum number of samples before a node split is forced. 0 means
	 * no limit.
	 * @param checkInterval contains number of samples required before each split check. Higher
	 * values check less often, which is more efficient, but may not split a node as early as
	 * possible.
	 * @param minSamples contains minimum number of samples for a node to see before a split is
	 * allowed.
	 */
	void train(
		std::vector<FlowFeatures>& data,
		std::vector<size_t>& labels,
		const std::string& path,
		size_t numClasses = 2,
		bool batchTraining = true,
		double successProbability = 0.95,
		size_t maxSamples = 0,
		size_t checkInterval = 100,
		size_t minSamples = 100);

private:
	/**
	 * @brief Hoeffding tree model
	 */
	mlpack::HoeffdingTree<> m_ht;
};

} // namespace WIF::MlpackModels
