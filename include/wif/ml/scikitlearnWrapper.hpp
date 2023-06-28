/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn wrapper interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <Python.h>
#include <numpy/arrayobject.h>

namespace WIF {

/**
 * @brief Wrapper class which provides a bridge to Python's Scikit-learn library
 *
 * @throw std::runtime_error if Python bridge fails to init properly
 */
class ScikitlearnWrapper {
public:
	using MlFeatures = std::vector<double>;

	/**
	 * @brief Construct a new Scikitlearn Wrapper object
	 *
	 * @param bridgePath path to Python script
	 * @param mlModelPath path to .pickle file with trained Machine Learning model
	 */
	ScikitlearnWrapper(const std::string& bridgePath, const std::string& mlModelPath);

	/**
	 * @brief Destroy the Scikitlearn Wrapper object
	 *
	 */
	~ScikitlearnWrapper();

	/**
	 * @brief Set the target probability idx
	 *
	 * Scikit-learn library's models return array of probabilities for each class
	 * targetClassProbabilityIdx is used to extract probability of the target class
	 *
	 * @param targetClassProbabilityIdx
	 */
	void setTargetClassProbabilityIdx(size_t targetClassProbabilityIdx);

	/**
	 * @brief Perform classification of features via the classify function from the bridge module
	 *
	 * @throw std::runtime_error when an error occurs during calls to Python's API
	 *
	 * @param burstOfFeatures vector of ML features
	 * @return std::vector<double>
	 */
	std::vector<double> classify(const std::vector<MlFeatures>& burstOfFeatures);

private:
	struct PyObjectDeleter {
		void operator()(PyObject* ptr) { Py_XDECREF(ptr); }
	};

	using PyObjectUniquePtr = std::unique_ptr<PyObject, PyObjectDeleter>;

	void init(const std::string& bridgePath, const std::string& mlModelPath);
	PyObjectUniquePtr loadBridge(const std::string& bridgePath) const;
	PyObjectUniquePtr loadFunction(PyObject* pyModule, const std::string& functionName) const;
	PyObjectUniquePtr loadModel(PyObject* bridgeInitFunction, const std::string& mlModelPath) const;

	inline PyObjectUniquePtr
	createFeaturesArray(const std::vector<MlFeatures>& burstOfFeatures) const;
	inline std::vector<double>
	readResultsFromReturnArray(PyObject* returnArray, size_t flowCount) const;

	std::pair<std::string, std::string> getDataForImport(const std::string& bridgePath) const;

	size_t m_targetClassProbabilityIdx = 0;
	PyObjectUniquePtr m_scikitMlModel = nullptr;
	PyObjectUniquePtr m_classifyFunction = nullptr;
};

} // namespace WIF
