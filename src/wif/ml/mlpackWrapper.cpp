/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Mlpack wrapper implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackWrapper.hpp"

namespace WIF {

MlpackWrapper::MlpackWrapper(const std::string& modelPath, const std::string& logicalName)
{
	initPrefixes();
	loadModel(modelPath, logicalName);
}

void MlpackWrapper::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	m_featuresIDs = sourceFeatureIDs;

	if (m_model && m_model->isLoaded()) {
		m_model->setFeatureSourceIDs(sourceFeatureIDs);
	} else {
		throw std::runtime_error("Mlpack model is not loaded.");
	}
}

const std::string MlpackWrapper::getModelPath() const
{
	return m_model->getPath();
}

bool MlpackWrapper::loadModel(const std::string& modelPath, const std::string& logicalName)
{
	std::string model;

	try {
		model = getModelNameFromPrefix(modelPath);
	} catch (...) {
		return false;
	}

	if (model == "NaiveBayesClassifier") {
		m_model = std::make_unique<WIF::MlpackModels::NaiveBayesModel>(modelPath, logicalName);
	} else if (model == "DecisionTree") {
		m_model = std::make_unique<WIF::MlpackModels::DecisionTreeModel>(modelPath, logicalName);
	} else if (model == "RandomForest") {
		m_model = std::make_unique<WIF::MlpackModels::RandomForestModel>(modelPath, logicalName);
	} else if (model == "LinearSVM") {
		m_model = std::make_unique<WIF::MlpackModels::LinearSVMModel>(modelPath, logicalName);
	} else if (model == "Perceptron") {
		m_model = std::make_unique<WIF::MlpackModels::PerceptronModel>(modelPath, logicalName);
	} else if (model == "HoeffdingTree") {
		m_model = std::make_unique<WIF::MlpackModels::HoeffdingTreeModel>(modelPath, logicalName);
	} else if (model == "Adaboost") {
		m_model = std::make_unique<WIF::MlpackModels::AdaBoostModel<mlpack::Perceptron<>>>(
			modelPath,
			logicalName);
	} else if (model == "Adaboost-DecisionTree") {
		m_model = std::make_unique<WIF::MlpackModels::AdaBoostModel<mlpack::DecisionTree<>>>(
			modelPath,
			logicalName);
	} else {
		return false;
	}

	if (!m_featuresIDs.empty() && m_model && m_model->isLoaded()) {
		m_model->setFeatureSourceIDs(m_featuresIDs);
	}

	return true;
}

ClfResult MlpackWrapper::classify(const FlowFeatures& flowFeatures)
{
	return m_model->classify(flowFeatures);
}

std::vector<ClfResult> MlpackWrapper::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	return m_model->classify(burstOfFeatures);
}

bool MlpackWrapper::isLoaded() const
{
	return m_model->isLoaded();
}

void MlpackWrapper::initPrefixes()
{
	m_prefixes.insert(std::pair<std::string, std::string>("nbc", "NaiveBayesClassifier"));
	m_prefixes.insert(std::pair<std::string, std::string>("dt", "DecisionTree"));
	m_prefixes.insert(std::pair<std::string, std::string>("rf", "RandomForest"));
	m_prefixes.insert(std::pair<std::string, std::string>("svm", "LinearSVM"));
	m_prefixes.insert(std::pair<std::string, std::string>("p", "Perceptron"));
	m_prefixes.insert(std::pair<std::string, std::string>("ht", "HoeffdingTree"));
	m_prefixes.insert(std::pair<std::string, std::string>("ab", "Adaboost"));
	m_prefixes.insert(std::pair<std::string, std::string>("abDt", "Adaboost-DecisionTree"));
}

std::string MlpackWrapper::getModelNameFromPrefix(const std::string& modelPath)
{
	auto lastSlashPos = modelPath.find_last_of("/\\");
	std::string modelFile;
	std::string model;

	if (lastSlashPos != std::string::npos) {
		modelFile = modelPath.substr(lastSlashPos + 1);
	} else {
		modelFile = modelPath;
	}

	auto underscorePos = modelFile.find('_');
	if (underscorePos != std::string::npos
		&& m_prefixes.find(modelFile.substr(0, underscorePos)) != m_prefixes.end()) {
		model = m_prefixes[modelFile.substr(0, underscorePos)];
	} else {
		throw std::runtime_error("Unrecognized model type.");
	}

	return model;
}

} // namespace WIF
