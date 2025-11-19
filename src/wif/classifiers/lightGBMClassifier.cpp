/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief LightGBM classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/lightGBMClassifier.hpp"

namespace WIF {

LightGBMClassifier::LightGBMClassifier(const std::string& path)
{
	m_lightGBMWrapper = std::make_unique<LightGBMWrapper>(path);
}

void LightGBMClassifier::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	Classifier::setFeatureSourceIDs(sourceFeatureIDs);
	m_lightGBMWrapper->setFeatureSourceIDs(sourceFeatureIDs);
}

ClfResult LightGBMClassifier::classify(const FlowFeatures& flowFeatures)
{
	return m_lightGBMWrapper->classify(flowFeatures);
}

std::vector<ClfResult>
LightGBMClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowFeatures)
{
	return m_lightGBMWrapper->classify(burstOfFlowFeatures);
}

const std::string& LightGBMClassifier::getMlModelPath() const noexcept
{
	return m_lightGBMWrapper->getModelPath();
}

void LightGBMClassifier::reloadModelFromDisk([[maybe_unused]] const std::string& logicalName)
{
	m_lightGBMWrapper->loadModel(m_lightGBMWrapper->getModelPath());
}

} // namespace WIF
