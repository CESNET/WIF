/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief ALF classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/alfClassifier.hpp"

namespace WIF {

AlfClassifier::AlfCallback::AlfCallback(const std::string& modelPath, AlfClassifier& classifier)
	: m_fileModificationChecker(modelPath)
	, m_classifier(classifier)
{
}

void AlfClassifier::AlfCallback::onTick()
{
	if (m_fileModificationChecker.isChangeDetected()) {
		m_classifier.markModelAsObsolete();
	}
}

AlfClassifier::AlfClassifier(
	ScikitMlClassifier& mlClassifier,
	UnirecReporter& reporter,
	unsigned timerIntervalInSeconds)
	: m_mlClassifier(mlClassifier)
	, m_reporter(reporter)
	, m_timer(
		  timerIntervalInSeconds,
		  std::make_unique<AlfCallback>(mlClassifier.getMlModelPath(), *this))
{
	updateLastModelLoadTime();
	m_timer.start();
}

ClfResult AlfClassifier::classify(const FlowFeatures& flowFeatures)
{
	return classify(std::vector<FlowFeatures> {flowFeatures})[0];
}

std::vector<ClfResult>
AlfClassifier::classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures)
{
	if (m_modelReloadNeeded) {
		handleModelUpdate();
	}

	auto burstOfResults = m_mlClassifier.classify(burstOfFlowsFeatures);
	for (unsigned flowIdx = 0; flowIdx < burstOfResults.size(); ++flowIdx) {
		auto& flowFeatures = burstOfFlowsFeatures[flowIdx];
		auto& results = burstOfResults[flowIdx];
		handleSingleReport(flowFeatures, results);
	}

	return burstOfResults;
}

void AlfClassifier::updateLastModelLoadTime()
{
	const auto timestamp = std::chrono::system_clock::now();
	m_lastModelLoadTime
		= std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count();
}

void AlfClassifier::handleModelUpdate()
{
	m_mlClassifier.reloadModelFromDisk();
	m_modelReloadNeeded = false;
	updateLastModelLoadTime();
}

void AlfClassifier::handleSingleReport(const FlowFeatures flowFeatures, const ClfResult& result)
{
	m_reporter.onRecordStart();

	// Identification fields are reported first
	for (const auto id : sourceFeatureIDs()) {
		m_reporter.report(flowFeatures.getRaw(id));
	}

	// ML features are reported as second
	for (const auto id : m_mlClassifier.sourceFeatureIDs()) {
		m_reporter.report(flowFeatures.getRaw(id));
	}

	// Report last time ML model was reloaded
	m_reporter.report({m_lastModelLoadTime});

	// ML proba array is reported as the last one
	m_reporter.report(result.get<std::vector<double>>());

	m_reporter.onRecordEnd();
}

} // namespace WIF
