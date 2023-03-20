/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn wrapper implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/scikitlearnWrapper.hpp"

namespace WIF {

ScikitlearnWrapper::ScikitlearnWrapper(
	const std::string& bridgePath,
	const std::string& mlModelPath)
{
	Py_Initialize();
	init(bridgePath, mlModelPath);
}

ScikitlearnWrapper::~ScikitlearnWrapper()
{
	PyRun_SimpleString("sys.stdout.flush()");
	PyRun_SimpleString("sys.stderr.flush()");
	Py_Finalize();
}

void ScikitlearnWrapper::setTargetClassProbabilityIdx(size_t targetClassProbabilityIdx)
{
	m_targetClassProbabilityIdx = targetClassProbabilityIdx;
}

void ScikitlearnWrapper::init(const std::string& bridgePath, const std::string& mlModelPath)
{
	if (_import_array() < 0) {
		throw std::runtime_error(
			"ScikitlearnWrapper::init() has failed. numpy.core.multiarray failed to import");
	}

	PyObjectUniquePtr pyModule = loadBridge(bridgePath);
	PyObjectUniquePtr initFunc = loadFunction(pyModule.get(), "init");

	m_scikitMlModel = loadModel(initFunc.get(), mlModelPath);
	m_classifyFunction = loadFunction(pyModule.get(), "classify");
}

ScikitlearnWrapper::PyObjectUniquePtr
ScikitlearnWrapper::loadBridge(const std::string& bridgePath) const
{
	auto [path, fileName] = this->getDataForImport(bridgePath);

	PyRun_SimpleString("import sys");
	PyRun_SimpleString(path.data());

	PyObject* pyModule = PyImport_Import(PyUnicode_FromString(fileName.data()));
	if (!pyModule) {
		throw std::runtime_error(
			"ScikitlearnWrapper::loadBridge() has failed. Bridge '" + bridgePath
			+ "' fails to load.");
	}

	return PyObjectUniquePtr(pyModule);
}

ScikitlearnWrapper::PyObjectUniquePtr
ScikitlearnWrapper::loadFunction(PyObject* pyModule, const std::string& functionName) const
{
	PyObject* func = PyObject_GetAttrString(pyModule, functionName.c_str());
	if (!func || !PyCallable_Check(func)) {
		throw std::runtime_error(
			"ScikitlearnWrapper::loadFunction() has failed. " + functionName + "() was not found!");
	}
	return PyObjectUniquePtr(func);
}

ScikitlearnWrapper::PyObjectUniquePtr
ScikitlearnWrapper::loadModel(PyObject* bridgeInitFunction, const std::string& mlModelPath) const
{
	PyObjectUniquePtr args = PyObjectUniquePtr(PyTuple_New(1));
	PyTuple_SetItem(args.get(), 0, PyUnicode_FromString(mlModelPath.c_str()));
	PyObject* mlModel = PyObject_CallObject(bridgeInitFunction, args.get());

	if (!mlModel) {
		throw std::runtime_error(
			"ScikitlearnWrapper::loadModel() has failed. '" + mlModelPath + "' not found!");
	}

	return PyObjectUniquePtr(mlModel);
}

ScikitlearnWrapper::PyObjectUniquePtr
ScikitlearnWrapper::createFeaturesArray(const std::vector<MlFeatures>& burstOfFeatures) const
{
	Py_ssize_t flowCount = (Py_ssize_t) burstOfFeatures.size();

	PyObjectUniquePtr featuresArray = PyObjectUniquePtr(PyTuple_New(flowCount));
	if (!featuresArray) {
		throw std::runtime_error(
			"ScikitlearnWrapper::createFeaturesArray() has failed. Python args array was not "
			"created!");
	}

	for (Py_ssize_t flowIdx = 0; flowIdx < flowCount; ++flowIdx) {
		Py_ssize_t featuresCount = (Py_ssize_t) burstOfFeatures[flowIdx].size();
		PyObject* mlFeatures = PyTuple_New(featuresCount);
		for (Py_ssize_t featureIdx = 0; featureIdx < featuresCount; featureIdx++) {
			PyTuple_SET_ITEM(
				mlFeatures,
				featureIdx,
				PyFloat_FromDouble(burstOfFeatures[flowIdx][featureIdx]));
		}
		PyTuple_SET_ITEM(featuresArray.get(), flowIdx, mlFeatures);
	}

	return featuresArray;
}

std::vector<double>
ScikitlearnWrapper::readResultsFromReturnArray(PyObject* returnArray, size_t flowCount) const
{
	std::vector<double> probabilities(flowCount, 0);
	for (size_t flowIdx = 0; flowIdx < flowCount; ++flowIdx) {
		probabilities[flowIdx] = PyFloat_AsDouble(
			PyList_GetItem(PyList_GetItem(returnArray, flowIdx), m_targetClassProbabilityIdx));
	}
	return probabilities;
}

std::vector<double> ScikitlearnWrapper::classify(const std::vector<MlFeatures>& burstOfFeatures)
{
	PyObjectUniquePtr featuresArray = createFeaturesArray(burstOfFeatures);
	PyObjectUniquePtr classifyArgs
		= PyObjectUniquePtr(Py_BuildValue("(OO)", m_scikitMlModel.get(), featuresArray.get()));
	if (!classifyArgs) {
		throw std::runtime_error(
			"ScikitlearnWrapper::classify() has failed. Python classify()'s arguments were not "
			"created!");
	}

	PyObjectUniquePtr classifyReturnArray
		= PyObjectUniquePtr(PyObject_CallObject(m_classifyFunction.get(), classifyArgs.get()));

	if (classifyReturnArray) {
		auto probabilities
			= readResultsFromReturnArray(classifyReturnArray.get(), burstOfFeatures.size());
		return probabilities;
	}

	throw std::runtime_error(
		"ScikitlearnWrapper::classify() has failed. Python classify() function call failed");
}

std::pair<std::string, std::string>
ScikitlearnWrapper::getDataForImport(const std::string& modulePath) const
{
	auto fileName = modulePath.substr(modulePath.find_last_of('/') + 1);
	std::string::size_type ext(fileName.find_last_of('.'));

	fileName = fileName.substr(0, ext);
	std::string::size_type p(modulePath.find_last_of('/') + 1);
	auto path = modulePath.substr(0, p);
	path = "sys.path.append(\"" + path + "\")";

	return {path, fileName};
}

} // namespace WIF
