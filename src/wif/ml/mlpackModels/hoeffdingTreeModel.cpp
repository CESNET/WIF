/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack Hoeffding tree model implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackModels/hoeffdingTreeModel.hpp"

namespace WIF::MlpackModels {

HoeffdingTreeModel::HoeffdingTreeModel(const std::string& modelPath)
{
	m_loaded = mlpack::data::Load(modelPath, "trained_data", m_ht, true);
}

ClfResult HoeffdingTreeModel::classify(const FlowFeatures& flowFeatures)
{
	arma::vec testData(m_featureIDs.size());
	arma::Row<size_t> predictions;
	MlpackModel::convertFeaturesToVector(flowFeatures, testData);
	m_ht.Classify(testData, predictions);
	size_t prediction = predictions(0);

	return ClfResult(static_cast<double>(prediction));
}

std::vector<ClfResult>
HoeffdingTreeModel::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	arma::mat testDataset(m_featureIDs.size(), burstOfFeatures.size());
	arma::Row<size_t> predictions;
	std::vector<ClfResult> burstResults;

	burstResults.reserve(burstOfFeatures.size());

	MlpackModel::convertBurstOfFeaturesToMatrix(burstOfFeatures, testDataset);
	m_ht.Classify(testDataset, predictions);
	for (const auto& prediction : predictions) {
		burstResults.emplace_back(static_cast<double>(prediction));
	}

	return burstResults;
}

bool HoeffdingTreeModel::load(const std::string& modelPath)
{
	m_loaded = mlpack::data::Load(modelPath, "trained_data", m_ht);
	return m_loaded;
}

bool HoeffdingTreeModel::save(const std::string& modelPath) const
{
	return mlpack::data::Save(modelPath, "trained_data", m_ht);
}

void HoeffdingTreeModel::train(
	std::vector<FlowFeatures>& data,
	std::vector<size_t>& labels,
	const std::string& path,
	size_t numClasses,
	bool batchTraining)
{
	arma::mat dataset(m_featureIDs.size(), data.size());
	arma::Row<size_t> armaLabels(labels);

	MlpackModel::convertBurstOfFeaturesToMatrix(data, dataset);
	m_ht.Train(dataset, armaLabels, numClasses, batchTraining, false);

	this->save(path);
}

} // namespace WIF::MlpackModels
