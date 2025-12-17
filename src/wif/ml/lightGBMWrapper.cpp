/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief LightGBM wrapper implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/lightGBMWrapper.hpp"

namespace WIF {

LightGBMWrapper::LightGBMWrapper()
{
	m_isLoaded = false;
}

LightGBMWrapper::LightGBMWrapper(const std::string& modelPath)
{
	if (!loadModel(modelPath)) {
		throw std::runtime_error("Failed to load model from file");
	}
}

LightGBMWrapper::~LightGBMWrapper()
{
	LGBM_BoosterFree(m_booster);
}

void LightGBMWrapper::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	m_featureIDs = sourceFeatureIDs;
}

const std::string& LightGBMWrapper::getModelPath() const
{
	return m_modelPath;
}

bool LightGBMWrapper::loadModel(const std::string& modelPath)
{
	if (!LGBM_BoosterCreateFromModelfile(modelPath.c_str(), &m_outNumIterations, &m_booster)) {
		m_isLoaded = true;
		m_modelPath = modelPath;
		return true;
	}

	return false;
}

ClfResult LightGBMWrapper::classify(const FlowFeatures& flowFeatures)
{
	std::vector<double> dataToClassify; // classified features from flowfeatures are extracted here
	int64_t outLen; // length of output result
	int numOfClasses; // number of classes
	LGBM_BoosterGetNumClasses(m_booster, &numOfClasses);
	std::vector<double> pred(numOfClasses); // vector with predictions

	for (const auto& featureID : m_featureIDs) {
		double value = flowFeatures.get<double>(featureID);
		dataToClassify.push_back(value);
	}

	LGBM_BoosterPredictForMatSingleRow(
		m_booster,
		dataToClassify.data(),
		C_API_DTYPE_FLOAT64,
		dataToClassify.size(),
		1,
		C_API_PREDICT_NORMAL,
		0,
		-1,
		"",
		&outLen,
		pred.data());

	if (numOfClasses == 1) {
		double tmp = pred[0];
		pred.insert(pred.begin(), (1.0 - tmp));
	}

	return ClfResult(pred);
}

std::vector<ClfResult> LightGBMWrapper::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	std::vector<double> dataToClassify; // Classified features from flowfeatures are extracted here
	int64_t outLen; // length of output result
	int numOfClasses; // number of classes
	std::vector<ClfResult> burstResults; // vector with predictions in ClfResult format

	burstResults.reserve(burstOfFeatures.size());
	LGBM_BoosterGetNumClasses(m_booster, &numOfClasses);

	std::vector<double> pred(burstOfFeatures.size() * numOfClasses); // vector with predictions

	for (const auto& feature : burstOfFeatures) { // data preparation for classification
		for (const auto& featureId : m_featureIDs) {
			double value = feature.get<double>(featureId);
			dataToClassify.push_back(value);
		}
	}

	LGBM_BoosterPredictForMat(
		m_booster,
		dataToClassify.data(),
		C_API_DTYPE_FLOAT64,
		burstOfFeatures.size(),
		m_featureIDs.size(),
		1,
		C_API_PREDICT_NORMAL,
		0,
		-1,
		"",
		&outLen,
		pred.data());

	for (unsigned idx = 0; idx < burstOfFeatures.size(); ++idx) { // converting pred to burstResults
		std::vector<double> probabilities(
			pred.begin() + idx * numOfClasses,
			pred.begin() + (idx + 1) * numOfClasses);

		if (numOfClasses == 1) {
			double tmp = probabilities[0];
			probabilities.insert(probabilities.begin(), (1.0 - tmp));
		}

		burstResults.emplace_back(probabilities);
	}

	return burstResults;
}

bool LightGBMWrapper::isLoaded() const
{
	return m_isLoaded;
}

void LightGBMWrapper::train(
	const std::string& datasetFileName,
	const char* datasetParams,
	const unsigned numOfIterations,
	const char* params,
	const std::string modelFileName)
{
	DatasetHandle trainData; // here the dataset with data for training will be loaded from file

	if (LGBM_DatasetCreateFromFile(
			datasetFileName.c_str(),
			datasetParams,
			nullptr,
			&trainData)) { // load dataset from file
		throw std::runtime_error("Error loading data");
	}

	if (LGBM_BoosterCreate(trainData, params, &m_booster) != 0) { // create a new boosting learner
		throw std::runtime_error("Error creating booster");
	}

	for (unsigned i = 0; i < numOfIterations; ++i) { // training
		int isFinished;
		LGBM_BoosterUpdateOneIter(m_booster, &isFinished);
		if (isFinished) {
			break;
		}
	}

	if (LGBM_BoosterSaveModel(m_booster, 0, -1, 0, modelFileName.c_str())) { // save model into file
		throw std::runtime_error("Error saving data");
	}
	m_isLoaded = true;
	m_modelPath = modelFileName;

	LGBM_DatasetFree(trainData);
}

} // namespace WIF
