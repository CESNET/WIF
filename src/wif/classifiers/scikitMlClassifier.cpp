/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn ML classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/scikitMlClassifier.hpp"

namespace WIF {

ScikitMlClassifier::ScikitMlClassifier(
	const std::string& bridgePath,
	const std::string& mlModelPath)
{
	m_scikitMlWrapper = std::make_unique<ScikitMlWrapper>(bridgePath, mlModelPath);
}

void ScikitMlClassifier::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	Classifier::setFeatureSourceIDs(sourceFeatureIDs);
	m_scikitMlWrapper->setFeatureSourceIDs(sourceFeatureIDs);
}

ClfResult ScikitMlClassifier::classify(const FlowFeatures& flowFeatures)
{
	return m_scikitMlWrapper->classify({flowFeatures})[0];
}

std::vector<ClfResult>
ScikitMlClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures)
{
	return m_scikitMlWrapper->classify(burstOfFlowsFeatures);
}

} // namespace WIF
