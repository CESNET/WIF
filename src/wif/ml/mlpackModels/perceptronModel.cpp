/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Perceptron model implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackModels/perceptronModel.hpp"

namespace WIF::MlpackModels {

PerceptronModel::PerceptronModel(const std::string& modelPath)
{
	m_loaded = mlpack::data::Load(modelPath, "trained_data", m_p, true);
	if (m_loaded) {
		m_modelPath = modelPath;
	}
}

ClfResult PerceptronModel::classify(const FlowFeatures& flowFeatures)
{
	arma::vec testData(m_featureIDs.size());
	arma::Row<size_t> predictions;
	MlpackModel::convertFeaturesToVector(flowFeatures, testData);
	m_p.Classify(testData, predictions);
	size_t prediction = predictions(0);

	return ClfResult(static_cast<double>(prediction));
}

std::vector<ClfResult> PerceptronModel::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	arma::mat testDataset(m_featureIDs.size(), burstOfFeatures.size());
	arma::Row<size_t> predictions;
	std::vector<ClfResult> burstResults;

	burstResults.reserve(burstOfFeatures.size());

	MlpackModel::convertBurstOfFeaturesToMatrix(burstOfFeatures, testDataset);
	m_p.Classify(testDataset, predictions);
	for (const auto& prediction : predictions) {
		burstResults.emplace_back(static_cast<double>(prediction));
	}

	return burstResults;
}

bool PerceptronModel::load(const std::string& modelPath)
{
	m_loaded = mlpack::data::Load(modelPath, "trained_data", m_p);
	if (m_loaded) {
		m_modelPath = modelPath;
	}
	return m_loaded;
}

bool PerceptronModel::save(const std::string& modelPath) const
{
	return mlpack::data::Save(modelPath, "trained_data", m_p);
}

void PerceptronModel::train(
	std::vector<FlowFeatures>& data,
	std::vector<size_t>& labels,
	const std::string& path,
	size_t numClasses,
	size_t maxIterations)
{
	arma::mat dataset(m_featureIDs.size(), data.size());
	arma::Row<size_t> armaLabels(labels);

	MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
	m_p.Train(dataset, armaLabels, numClasses, maxIterations);

	this->save(path);
}

void PerceptronModel::train(
	std::vector<FlowFeatures>& data,
	std::vector<size_t>& labels,
	const std::string& path,
	const arma::rowvec& weights,
	size_t numClasses,
	size_t maxIterations)
{
	arma::mat dataset(m_featureIDs.size(), data.size());
	arma::Row<size_t> armaLabels(labels);

	MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
	m_p.Train(dataset, armaLabels, numClasses, weights, maxIterations);

	this->save(path);
}

} // namespace WIF::MlpackModels
