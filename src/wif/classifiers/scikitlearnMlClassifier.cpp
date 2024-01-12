/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn ML classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/scikitlearnMlClassifier.hpp"

namespace WIF {

ScikitlearnMlClassifier::ScikitlearnMlClassifier(
	const std::string& bridgePath,
	const std::string& mlModelPath)
{
	m_scikitlearnWrapper = std::make_unique<ScikitlearnWrapper>(bridgePath, mlModelPath);
}

void ScikitlearnMlClassifier::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	Classifier::setFeatureSourceIDs(sourceFeatureIDs);
	m_scikitlearnWrapper->setFeatureSourceIDs(sourceFeatureIDs);
}

ClfResult ScikitlearnMlClassifier::classify(const FlowFeatures& flowFeatures)
{
	return m_scikitlearnWrapper->classify({flowFeatures})[0];
}

std::vector<ClfResult>
ScikitlearnMlClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures)
{
	return m_scikitlearnWrapper->classify(burstOfFlowsFeatures);
}

} // namespace WIF
