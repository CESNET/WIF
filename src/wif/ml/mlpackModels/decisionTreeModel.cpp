/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Decision tree model implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackModels/decisionTreeModel.hpp"

namespace WIF::MlpackModels {

DecisionTreeModel::DecisionTreeModel(const std::string& modelPath)
{
	m_loaded = mlpack::data::Load(modelPath, "trained_data", m_dt, true);
}

ClfResult DecisionTreeModel::classify(const FlowFeatures& flowFeatures)
{
	arma::vec testData(m_featureIDs.size());
	size_t prediction;
	arma::vec probaVec;
	MlpackModel::convertFeaturesToVector(flowFeatures, testData);
	m_dt.Classify(testData, prediction, probaVec);
	std::vector<double> probabilities(probaVec.begin(), probaVec.end());

	return ClfResult(probabilities);
}

std::vector<ClfResult> DecisionTreeModel::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	arma::mat testDataset(m_featureIDs.size(), burstOfFeatures.size());
	arma::Row<size_t> predictions;
	arma::mat probaMatrix;
	std::vector<ClfResult> burstResults;

	burstResults.reserve(burstOfFeatures.size());

	MlpackModel::convertBurstOfFeaturesToMatrix(burstOfFeatures, testDataset);
	m_dt.Classify(testDataset, predictions, probaMatrix);
	for (size_t i = 0; i < predictions.n_elem; ++i) {
		std::vector<double> probabilities(probaMatrix.col(i).begin(), probaMatrix.col(i).end());
		burstResults.emplace_back(probabilities);
	}

	return burstResults;
}

bool DecisionTreeModel::load(const std::string& modelPath)
{
	m_loaded = mlpack::data::Load(modelPath, "trained_data", m_dt);
	return m_loaded;
}

bool DecisionTreeModel::save(const std::string& modelPath) const
{
	return mlpack::data::Save(modelPath, "trained_data", m_dt);
}

void DecisionTreeModel::train(
	std::vector<FlowFeatures>& data,
	std::vector<size_t>& labels,
	const std::string& path,
	size_t numClasses,
	size_t minLeafSize,
	double minGainSplit,
	size_t maxDepth)
{
	arma::mat dataset(m_featureIDs.size(), data.size());
	arma::Row<size_t> armaLabels(labels);

	MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
	m_dt.Train(dataset, armaLabels, numClasses, minLeafSize, minGainSplit, maxDepth);

	this->save(path);
}

} // namespace WIF::MlpackModels
