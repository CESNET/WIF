/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack AdaBoost model class
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackModels/mlpackModel.hpp"
#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

#include <armadillo>
#include <mlpack.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace WIF::MlpackModels {

/**
 * @brief Class which provides AdaBoost model weak learner from Mlpack library
 * @tparam WeakLearnerType weak learner used by AdaBoost
 */
template<typename WeakLearnerType>
class AdaBoostModel : public MlpackModel {
public:
	/**
	 * @brief Construct a new AdaBoost wrapper object with no loaded model
	 */
	AdaBoostModel() = default;

	/**
	 * @brief Construct a new AdaBoost wrapper object
	 *
	 * @param modelPath contains path to trained model file
	 * @param logicalName contains the logical name of the trained model.
	 */
	AdaBoostModel(const std::string& modelPath, const std::string& logicalName = "trained_data")
	{
		m_loaded = mlpack::data::Load(modelPath, logicalName, m_ab, true);
		if (m_loaded) {
			m_modelPath = modelPath;
		}
	}

	/**
	 * @brief Classify single flowFeature object
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification, which contains vector<double> with
	 * probabilities for each class.
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override
	{
		arma::mat testDataset(m_featureIDs.size(), 1);
		arma::Row<size_t> predictions;
		arma::mat probaMatrix;
		std::vector<ClfResult> burstResults;
		std::vector<FlowFeatures> burstOfFeatures = {flowFeatures};

		burstResults.reserve(1);

		MlpackModel::convertBurstOfFeaturesToMatrix(burstOfFeatures, testDataset);
		m_ab.Classify(testDataset, predictions, probaMatrix);
		for (unsigned i = 0; i < predictions.n_elem; ++i) {
			std::vector<double> probabilities(probaMatrix.col(i).begin(), probaMatrix.col(i).end());
			burstResults.emplace_back(probabilities);
		}

		return burstResults[0];
	}

	/**
	 * @brief Classify a burst of flow features
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> the results of the classification. Each ClfResult contains
	 * result of the classification, which contains vector<double> with probabilities for each
	 * class
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFeatures) override
	{
		arma::mat testDataset(m_featureIDs.size(), burstOfFeatures.size());
		arma::Row<size_t> predictions;
		arma::mat probaMatrix;
		std::vector<ClfResult> burstResults;

		burstResults.reserve(burstOfFeatures.size());

		MlpackModel::convertBurstOfFeaturesToMatrix(burstOfFeatures, testDataset);
		m_ab.Classify(testDataset, predictions, probaMatrix);
		for (unsigned i = 0; i < predictions.n_elem; ++i) {
			std::vector<double> probabilities(probaMatrix.col(i).begin(), probaMatrix.col(i).end());
			burstResults.emplace_back(probabilities);
		}

		return burstResults;
	}

	/**
	 * @brief Load AdaBoost model from file
	 *
	 * @param modelPath contains path to the model file.
	 * @param logicalName contains the logical name of the trained model.
	 * @return Bool value true, if model was successfully loaded. False if not.
	 */
	bool
	load(const std::string& modelPath, const std::string& logicalName = "trained_data") override
	{
		m_loaded = mlpack::data::Load(modelPath, logicalName, m_ab);
		if (m_loaded) {
			m_modelPath = modelPath;
		}
		return m_loaded;
	}

	/**
	 * @brief Save AdaBoost model to file
	 *
	 * @param modelPath contains file path, where the model will be saved.
	 * @param logicalName contains the logical name of the trained model.
	 * @return Bool value true, if model was successfully saved. False if not.
	 */
	bool save(const std::string& modelPath, const std::string& logicalName = "trained_data")
		const override
	{
		return mlpack::data::Save(modelPath, logicalName, m_ab);
	}

	/**
	 * @brief Train AdaBoost model
	 *
	 * @param data contains training vector of flow features.
	 * @param labels contains training labels, between 0 and numClasses - 1 (inclusive). Should have
	 * length data.length().
	 * @param path contains path, where file will be saved.
	 * @param numClasses contains number of classes in the dataset.
	 * @param maxIterations contains maximum number of iterations of AdaBoost.MH to use. This is the
	 * maximum number of weak learners to train. (0 means no limit, and weak learners will be
	 * trained until the tolerance is met.)
	 * @param tolerance when the weighted residual (r_t) of the model goes below tolerance, training
	 * will terminate and no more weak learners will be added.
	 * @param weakLearnerParams optional weak learner hyperparameters.
	 */
	template<typename... WeakLearnerParams>
	void train(
		const std::vector<FlowFeatures>& data,
		const std::vector<size_t>& labels,
		const std::string& path,
		size_t numClasses = 2,
		size_t maxIterations = 100,
		double tolerance = 1e-6,
		WeakLearnerParams&&... weakLearnerParams)
	{
		arma::mat dataset(m_featureIDs.size(), data.size());
		arma::Row<size_t> armaLabels(labels);

		MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
		m_ab.Train(
			dataset,
			armaLabels,
			numClasses,
			maxIterations,
			tolerance,
			std::forward<WeakLearnerParams>(weakLearnerParams)...);

		this->save(path);
	}

private:
	/**
	 * @brief AdaBoost model
	 */
	mlpack::AdaBoost<WeakLearnerType, arma::mat> m_ab;
};

} // namespace WIF::MlpackModels
