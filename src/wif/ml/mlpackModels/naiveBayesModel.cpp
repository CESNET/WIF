/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Naive Bayes model implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackModels/naiveBayesModel.hpp"

namespace WIF::MlpackModels {

NaiveBayesModel::NaiveBayesModel(const std::string& modelPath, const std::string& logicalName)
{
	m_loaded = mlpack::data::Load(modelPath, logicalName, m_nbc, true);
	if (m_loaded) {
		m_modelPath = modelPath;
	}
}

ClfResult NaiveBayesModel::classify(const FlowFeatures& flowFeatures)
{
	arma::vec testData(m_featureIDs.size());
	size_t prediction;
	arma::vec probaVec;
	MlpackModel::convertFeaturesToVector(flowFeatures, testData);
	m_nbc.Classify(testData, prediction, probaVec);
	std::vector<double> probabilities(probaVec.begin(), probaVec.end());

	return ClfResult(probabilities);
}

std::vector<ClfResult> NaiveBayesModel::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	arma::mat testDataset(m_featureIDs.size(), burstOfFeatures.size());
	arma::Row<size_t> predictions;
	arma::mat probaMatrix;
	std::vector<ClfResult> burstResults;

	burstResults.reserve(burstOfFeatures.size());

	MlpackModel::convertBurstOfFeaturesToMatrix(burstOfFeatures, testDataset);
	m_nbc.Classify(testDataset, predictions, probaMatrix);
	for (size_t i = 0; i < predictions.n_elem; ++i) {
		std::vector<double> probabilities(probaMatrix.col(i).begin(), probaMatrix.col(i).end());
		burstResults.emplace_back(probabilities);
	}

	return burstResults;
}

bool NaiveBayesModel::load(const std::string& modelPath, const std::string& logicalName)
{
	m_loaded = mlpack::data::Load(modelPath, logicalName, m_nbc);
	if (m_loaded) {
		m_modelPath = modelPath;
	}
	return m_loaded;
}

bool NaiveBayesModel::save(const std::string& modelPath, const std::string& logicalName) const
{
	return mlpack::data::Save(modelPath, logicalName, m_nbc);
}

void NaiveBayesModel::train(
	const std::vector<FlowFeatures>& data,
	const std::vector<size_t>& labels,
	const std::string& path,
	size_t numClasses)
{
	arma::mat dataset(m_featureIDs.size(), data.size());
	arma::Row<size_t> armaLabels(labels);

	MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
	m_nbc.Train(dataset, armaLabels, numClasses);

	this->save(path);
}

} // namespace WIF::MlpackModels
