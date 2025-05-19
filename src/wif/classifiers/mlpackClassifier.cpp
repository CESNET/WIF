/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/mlpackClassifier.hpp"

namespace WIF {

MlpackClassifier::MlpackClassifier(const std::string& path, const std::string& logicalName)
{
	m_mlpackWrapper = std::make_unique<MlpackWrapper>(path, logicalName);
}

void MlpackClassifier::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	Classifier::setFeatureSourceIDs(sourceFeatureIDs);
	m_mlpackWrapper->setFeatureSourceIDs(sourceFeatureIDs);
}

ClfResult MlpackClassifier::classify(const FlowFeatures& flowFeatures)
{
	return m_mlpackWrapper->classify(flowFeatures);
}

std::vector<ClfResult>
MlpackClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowFeatures)
{
	return m_mlpackWrapper->classify(burstOfFlowFeatures);
}

const std::string MlpackClassifier::getMlModelPath() const noexcept
{
	return m_mlpackWrapper->getModelPath();
}

void MlpackClassifier::reloadModelFromDisk(const std::string& logicalName)
{
	m_mlpackWrapper->loadModel(m_mlpackWrapper->getModelPath(), logicalName);
}

} // namespace WIF
