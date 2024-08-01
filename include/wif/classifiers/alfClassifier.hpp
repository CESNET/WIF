/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief ALF classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/scikitMlClassifier.hpp"
#include "wif/filesystem/fileModificationChecker.hpp"
#include "wif/reporters/unirecReporter.hpp"
#include "wif/utils/timer.hpp"

#include <chrono>
#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Classifier performing ML classification which is interconnected with ALF
 *
 */
class AlfClassifier : public Classifier {
private:
	class AlfCallback : public TimerCallback {
	public:
		AlfCallback(const std::string& modelPath, AlfClassifier& classifier);

		void onTick() override;

	private:
		FileModificationChecker m_fileModificationChecker;
		AlfClassifier& m_classifier;
	};

public:
	/**
	 * @brief Construct a new Alf Classifier object
	 *
	 * @param mlClassifier underlying ML classifier, which has already set source feature IDs
	 * @param reporter UniRec reporter for ALF interconnection, which already has its interface
	 * ready
	 * @param timerIntervalInSeconds number of seconds between each check of file on disk
	 */
	AlfClassifier(
		ScikitMlClassifier& mlClassifier,
		UnirecReporter& reporter,
		unsigned timerIntervalInSeconds);

	/**
	 * @brief Classify single flowFeature object
	 * See std::vector<ClfResult> classify(const std::vector<FlowFeatures>&) for more details
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult result of the classification
	 */
	ClfResult classify(const FlowFeatures& flowFeatures) override;

	/**
	 * @brief Classify a burst of flow features
	 * Firstly, ML model is reloaded if needed, then ML classifier is used to obtain results
	 * Then, source features of ALF classifier are reported, then source features of ML classifier
	 * are reported, then last model load time (uint64_t) and finally classification results are
	 * reported
	 *
	 * @param burstOfFlowsFeatures the burst of flow features to classify
	 * @return std::vector<ClfResult> the results of the classification
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFlowsFeatures) override;

	/**
	 * @brief Method which marks used ML model as obsolete
	 * This operation will result into ML model reload during the next call of classify() method
	 */
	void markModelAsObsolete() { m_modelReloadNeeded = true; }

private:
	void updateLastModelLoadTime();
	void handleModelUpdate();
	void handleSingleReport(const FlowFeatures flowFeatures, const ClfResult& result);

	bool m_modelReloadNeeded = false;
	uint64_t m_lastModelLoadTime;
	ScikitMlClassifier& m_mlClassifier;
	UnirecReporter& m_reporter;
	Timer m_timer;
};

} // namespace WIF
