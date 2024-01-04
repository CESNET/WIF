/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Scikit-learn wrapper interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/clfResult.hpp"

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
 * @throw std::runtime_error if Python initialization fails to init properly
 */
class ScikitlearnWrapper {
	struct PyObjectDeleter {
		void operator()(PyObject* ptr) { Py_XDECREF(ptr); }
	};

public:
	using PyObjectUniquePtr = std::unique_ptr<PyObject, PyObjectDeleter>;
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
	 * @brief Getter for used ML model path
	 * @return const std::string&
	 */
	const std::string& mlModelPath() const noexcept { return m_mlModelPath; }

	/**
	 * @brief Perform classification of features via the classify function from the bridge
	 * module
	 *
	 * @throw std::runtime_error when an error occurs during calls to Python's API
	 *
	 * @param burstOfFeatures vector of ML features
	 * @return std::vector<double>
	 */
	std::vector<ClfResult> classify(const std::vector<MlFeatures>& burstOfFeatures);

private:
	void init();
	PyObjectUniquePtr loadBridge() const;
	PyObjectUniquePtr loadFunction(const std::string& functionName) const;
	PyObjectUniquePtr loadModel() const;
	PyObjectUniquePtr callClassifyMethod(PyObject* args);

	size_t m_targetClassProbabilityIdx = 0;
	std::string m_bridgePath;
	std::string m_mlModelPath;
	PyObjectUniquePtr m_bridgeModule = nullptr;
	PyObjectUniquePtr m_initFunction = nullptr;
	PyObjectUniquePtr m_scikitMlModel = nullptr;
	PyObjectUniquePtr m_classifyFunction = nullptr;
};

} // namespace WIF
