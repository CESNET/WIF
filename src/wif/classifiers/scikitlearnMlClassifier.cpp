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
	const std::string& mlModelPath,
	size_t targetClassProbabilityIdx)
	: m_scikitlearnWrapper(std::make_unique<ScikitlearnWrapper>(bridgePath, mlModelPath))
{
	m_scikitlearnWrapper->setTargetClassProbabilityIdx(targetClassProbabilityIdx);
}

double ScikitlearnMlClassifier::classify(const FlowFeatures& flowFeatures)
{
	return m_scikitlearnWrapper->classify({createFeaturesFromFlow(flowFeatures)})[0];
}

std::vector<double>
ScikitlearnMlClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures)
{
	std::vector<MlFeatures> burstOfFeatures;
	burstOfFeatures.reserve(burstOfFlowsFeatures.size());

	for (const auto& flowFeatures : burstOfFlowsFeatures) {
		burstOfFeatures.emplace_back(createFeaturesFromFlow(flowFeatures));
	}

	return m_scikitlearnWrapper->classify(burstOfFeatures);
}

ScikitlearnMlClassifier::MlFeatures
ScikitlearnMlClassifier::createFeaturesFromFlow(const FlowFeatures& flowFeatures) const
{
	const auto& featureIDs = sourceFeatureIDs();

	MlFeatures features;
	features.reserve(featureIDs.size());
	for (size_t featureIdx = 0; featureIdx < featureIDs.size(); ++featureIdx) {
		features.emplace_back(flowFeatures.get<double>(featureIDs[featureIdx]));
	}

	return features;
}

} // namespace WIF
