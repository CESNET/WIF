/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn wrapper implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/scikitlearnWrapper.hpp"

namespace WIF {

using PyObjectUniquePtr = ScikitlearnWrapper::PyObjectUniquePtr;

namespace {

std::pair<std::string, std::string> parseModulePath(const std::string& modulePath)
{
	auto fileName = modulePath.substr(modulePath.find_last_of('/') + 1);
	std::string::size_type ext(fileName.find_last_of('.'));

	fileName = fileName.substr(0, ext);
	std::string::size_type p(modulePath.find_last_of('/') + 1);
	auto path = modulePath.substr(0, p);

	return {path, fileName};
}

PyObjectUniquePtr buildArgsForClassifyFunction(PyObject* mlModelPtr, PyObject* featuresArray)
{
	return PyObjectUniquePtr(Py_BuildValue("(OO)", mlModelPtr, featuresArray));
}

ClfResult parseBurstOfProbaArray(PyObject* array)
{
	std::vector<double> probabilities;
	probabilities.reserve(PyList_Size(array));

	for (unsigned probaIdx = 0; probaIdx < PyList_Size(array); ++probaIdx) {
		auto proba = PyList_GetItem(array, probaIdx);
		probabilities.emplace_back(PyFloat_AsDouble(proba));
	}

	return ClfResult(probabilities);
}

std::vector<ClfResult> parseBurstOfProbaArrays(PyObject* array)
{
	std::vector<ClfResult> probabilities;
	probabilities.reserve(PyList_Size(array));

	for (unsigned burstIdx = 0; burstIdx < PyList_Size(array); ++burstIdx) {
		auto clfResult = parseBurstOfProbaArray(PyList_GetItem(array, burstIdx));
		probabilities.emplace_back(clfResult);
	}

	return probabilities;
}

} // namespace

ScikitlearnWrapper::ScikitlearnWrapper(
	const std::string& bridgePath,
	const std::string& mlModelPath)
	: m_bridgePath(bridgePath)
	, m_mlModelPath(mlModelPath)
{
	Py_Initialize();
	init();
}

ScikitlearnWrapper::~ScikitlearnWrapper()
{
	// Reset unique pointers so Py_XDECREF is called before Python finalization
	m_bridgeModule.reset();
	m_initFunction.reset();
	m_scikitMlModel.reset();
	m_classifyFunction.reset();

	PyRun_SimpleString("sys.stdout.flush()");
	PyRun_SimpleString("sys.stderr.flush()");
	Py_Finalize();
}

void ScikitlearnWrapper::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	m_featureIDs = sourceFeatureIDs;
}

void ScikitlearnWrapper::init()
{
	if (_import_array() < 0) {
		throw std::runtime_error(
			"ScikitlearnWrapper::init() has failed. numpy.core.multiarray failed to import");
	}

	m_bridgeModule = loadBridge();
	m_initFunction = loadFunction("init");
	m_classifyFunction = loadFunction("classify");
	m_scikitMlModel = loadModel();
}

void ScikitlearnWrapper::reloadModel()
{
	m_scikitMlModel = loadModel();
}

PyObjectUniquePtr ScikitlearnWrapper::loadBridge() const
{
	auto [path, fileName] = parseModulePath(m_bridgePath);
	auto pathAppendString = "sys.path.append(\"" + path + "\")";

	PyRun_SimpleString("import sys");
	PyRun_SimpleString(pathAppendString.data());

	PyObject* pyModule = PyImport_Import(PyUnicode_FromString(fileName.data()));
	if (!pyModule) {
		throw std::runtime_error(
			"ScikitlearnWrapper::loadBridge() has failed. Bridge '" + m_bridgePath
			+ "' fails to load.");
	}

	return PyObjectUniquePtr(pyModule);
}

PyObjectUniquePtr ScikitlearnWrapper::loadFunction(const std::string& functionName) const
{
	PyObject* func = PyObject_GetAttrString(m_bridgeModule.get(), functionName.c_str());
	if (!func || !PyCallable_Check(func)) {
		throw std::runtime_error(
			"ScikitlearnWrapper::loadFunction() has failed. " + functionName + "() was not found!");
	}
	return PyObjectUniquePtr(func);
}

PyObjectUniquePtr ScikitlearnWrapper::loadModel() const
{
	PyObjectUniquePtr args = PyObjectUniquePtr(PyTuple_New(1));
	PyTuple_SetItem(args.get(), 0, PyUnicode_FromString(m_mlModelPath.c_str()));
	PyObject* mlModel = PyObject_CallObject(m_initFunction.get(), args.get());

	if (!mlModel) {
		throw std::runtime_error(
			"ScikitlearnWrapper::loadModel() has failed. '" + m_mlModelPath + "' not found!");
	}

	return PyObjectUniquePtr(mlModel);
}

PyObjectUniquePtr ScikitlearnWrapper::callClassifyMethod(PyObject* args)
{
	return PyObjectUniquePtr(PyObject_CallObject(m_classifyFunction.get(), args));
}

std::vector<ClfResult>
ScikitlearnWrapper::classify(const std::vector<FlowFeatures>& burstOfFeatures)
{
	auto featuresArray = createArrayOfBurstsOfFeatures(burstOfFeatures);
	auto arguments = buildArgsForClassifyFunction(m_scikitMlModel.get(), featuresArray.get());
	auto returnedArray = callClassifyMethod(arguments.get());
	return parseBurstOfProbaArrays(returnedArray.get());
}

PyObject* ScikitlearnWrapper::createArrayOfFeatures(const FlowFeatures& features) const
{
	PyObject* pyArray = PyList_New((Py_ssize_t) m_featureIDs.size());
	Py_ssize_t featureIdx = 0;
	for (const auto& featureID : m_featureIDs) {
		double value = features.get<double>(featureID);
		PyList_SetItem(pyArray, featureIdx, PyFloat_FromDouble(value));
		++featureIdx;
	}
	return pyArray;
}

PyObjectUniquePtr ScikitlearnWrapper::createArrayOfBurstsOfFeatures(
	const std::vector<FlowFeatures>& burstOfFeatures) const
{
	PyObject* featuresArray = PyList_New((Py_ssize_t) burstOfFeatures.size());
	Py_ssize_t burstIndex = 0;
	for (const auto& features : burstOfFeatures) {
		PyList_SetItem(featuresArray, burstIndex, createArrayOfFeatures(features));
		++burstIndex;
	}
	return PyObjectUniquePtr(featuresArray);
}

} // namespace WIF
