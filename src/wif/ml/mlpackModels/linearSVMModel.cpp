/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack linear SVM model implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackModels/linearSVMModel.hpp"

namespace WIF::MlpackModels {

LinearSVMModel::LinearSVMModel(const std::string& modelPath, const std::string& logicalName)
{
	m_loaded = mlpack::data::Load(modelPath, logicalName, m_svm, true);
	if (m_loaded) {
		m_modelPath = modelPath;
	}
}

ClfResult LinearSVMModel::classify(const FlowFeatures& flowFeatures)
{
	arma::vec testData(m_featureIDs.size());
	size_t prediction;
	arma::vec probaVec;
	MlpackModel::convertFeaturesToVector(flowFeatures, testData);
	m_svm.Classify(testData, prediction, probaVec);
	std::vector<double> probabilities(probaVec.begin(), probaVec.end());

	return ClfResult(probabilities);
}

std::vector<ClfResult> LinearSVMModel::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	arma::mat testDataset(m_featureIDs.size(), burstOfFeatures.size());
	arma::Row<size_t> predictions;
	arma::mat probaMatrix;
	std::vector<ClfResult> burstResults;

	burstResults.reserve(burstOfFeatures.size());

	MlpackModel::convertBurstOfFeaturesToMatrix(burstOfFeatures, testDataset);
	m_svm.Classify(testDataset, predictions, probaMatrix);
	for (size_t i = 0; i < predictions.n_elem; ++i) {
		std::vector<double> probabilities(probaMatrix.col(i).begin(), probaMatrix.col(i).end());
		burstResults.emplace_back(probabilities);
	}

	return burstResults;
}

bool LinearSVMModel::load(const std::string& modelPath, const std::string& logicalName)
{
	m_loaded = mlpack::data::Load(modelPath, logicalName, m_svm);
	if (m_loaded) {
		m_modelPath = modelPath;
	}
	return m_loaded;
}

bool LinearSVMModel::save(const std::string& modelPath, const std::string& logicalName) const
{
	return mlpack::data::Save(modelPath, logicalName, m_svm);
}

} // namespace WIF::MlpackModels
