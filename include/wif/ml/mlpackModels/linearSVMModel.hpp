/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack linear SVM model interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/ml/mlpackModels/mlpackModel.hpp"
#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

#include <armadillo>
#include <ensmallen.hpp>
#include <memory>
#include <mlpack/core.hpp>
#include <mlpack/methods/linear_svm/linear_svm.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace WIF::MlpackModels {

/**
 * @brief Class which provides linear SVM model of Mlpack library
 */
class LinearSVMModel : public MlpackModel {
public:
	/**
	 * @brief Construct a new linear SVM object with no loaded model
	 */
	LinearSVMModel() = default;

	/**
	 * @brief Construct a new linear SVM object
	 *
	 * @param modelPath contains path to trained model file.
	 * @param logicalName contains the logical name of the trained model.
	 */
	LinearSVMModel(const std::string& modelPath, const std::string& logicalName = "trained_data");

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
	 * @brief Load linear SVM model from file
	 *
	 * @param modelPath contains path to model file.
	 * @param logicalName contains the logical name of the trained model.
	 * @return Bool value true, if model was successfully loaded. False if not.
	 */
	bool
	load(const std::string& modelPath, const std::string& logicalName = "trained_data") override;

	/**
	 * @brief Save linear SVM trained model to file
	 *
	 * @param modelPath contains path, where the model will be saved.
	 * @param logicalName contains the logical name of the trained model.
	 * @return Bool value true, if model was successfully saved. False if not.
	 */
	bool save(const std::string& modelPath, const std::string& logicalName = "trained_data")
		const override;

	/**
	 * @brief Train linear SVM model
	 *
	 * @param data contains training vector of flow features.
	 * @param labels contains training labels, between 0 and numClasses - 1 (inclusive). Should have
	 * length data.length().
	 * @param path contains path, where file will be saved.
	 * @param numClasses contains number of classes in the dataset.
	 * @param lambda contains L2 regularization penalty parameter. Must be nonnegative.
	 * @param delta contains margin of difference between correct class and other classes.
	 * @param fitIntercept is true, if an intercept term is fitted to the model.
	 * @param callbacks contains optional callbacks for the ensmallen optimizer, such as e.g.
	 * ens::ProgressBar(), ens::Report(), or others.
	 */
	template<typename... CallbackTypes>
	void train(
		std::vector<FlowFeatures>& data,
		std::vector<size_t>& labels,
		const std::string& path,
		size_t numClasses = 2,
		double lambda = 0.0001,
		double delta = 1.0,
		bool fitIntercept = false,
		CallbackTypes&&... callbacks)
	{
		arma::mat dataset(m_featureIDs.size(), data.size());
		arma::Row<size_t> armaLabels(labels);

		MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
		m_svm.Train(
			dataset,
			armaLabels,
			numClasses,
			lambda,
			delta,
			fitIntercept,
			std::forward<CallbackTypes>(callbacks)...);

		this->save(path);
	}

	/**
	 * @brief Train linear SVM model
	 *
	 * @param data contains training vector of flow features.
	 * @param labels contains training labels, between 0 and numClasses - 1 (inclusive). Should have
	 * length data.length().
	 * @param path contains path, where file will be saved.
	 * @param numClasses contains number of classes in the dataset.
	 * @param optimizer contains instantiated ensmallen optimizer for differentiable functions or
	 * differentiable separable functions.
	 * @param lambda contains L2 regularization penalty parameter. Must be nonnegative.
	 * @param delta contains margin of difference between correct class and other classes.
	 * @param fitIntercept is true, if an intercept term is fitted to the model.
	 * @param callbacks contains optional callbacks for the ensmallen optimizer, such as e.g.
	 * ens::ProgressBar(), ens::Report(), or others.
	 */
	template<typename OptimizerType, typename... CallbackTypes>
	void train(
		const std::vector<FlowFeatures>& data,
		const std::vector<size_t>& labels,
		const std::string& path,
		size_t numClasses,
		OptimizerType& optimizer,
		double lambda = 0.0001,
		double delta = 1.0,
		bool fitIntercept = false,
		CallbackTypes&&... callbacks)
	{
		arma::mat dataset(m_featureIDs.size(), data.size());
		arma::Row<size_t> armaLabels(labels);

		MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
		m_svm.Train(
			dataset,
			armaLabels,
			numClasses,
			optimizer,
			lambda,
			delta,
			fitIntercept,
			std::forward<CallbackTypes>(callbacks)...);

		this->save(path);
	}

private:
	/**
	 * @brief Linear SVM model
	 */
	mlpack::LinearSVM<> m_svm;
};

} // namespace WIF::MlpackModels
