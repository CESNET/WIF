/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Scikit-learn wrapper interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

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
class ScikitMlWrapper {
	struct PyObjectDeleter {
		void operator()(PyObject* ptr) { Py_XDECREF(ptr); }
	};

public:
	using PyObjectUniquePtr = std::unique_ptr<PyObject, PyObjectDeleter>;

	/**
	 * @brief Construct a new Scikitlearn Wrapper object
	 *
	 * @param bridgePath path to Python script
	 * @param mlModelPath path to .pickle file with trained Machine Learning model
	 */
	ScikitMlWrapper(const std::string& bridgePath, const std::string& mlModelPath);

	/**
	 * @brief Destroy the Scikitlearn Wrapper object
	 *
	 */
	~ScikitMlWrapper();

	/**
	 * @brief Set feature IDs which will be used for classification
	 *
	 * @param sourceFeatureIDs
	 */
	void setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs);

	/**
	 * @brief Getter for path of the used ML model
	 * @return const std::string&
	 */
	const std::string& getMlModelPath() const noexcept { return m_mlModelPath; }

	/**
	 * @brief Reload model from disk
	 *
	 */
	void reloadModel();

	/**
	 * @brief Perform classification of features via the classify function from the bridge
	 * module
	 *
	 * @throw std::runtime_error when an error occurs during calls to Python's API
	 *
	 * @param burstOfFeatures vector of ML features
	 * @return std::vector<ClfResult>
	 */
	std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFeatures);

private:
	void init();
	PyObjectUniquePtr loadBridge() const;
	PyObjectUniquePtr loadFunction(const std::string& functionName) const;
	PyObjectUniquePtr loadModel() const;
	PyObjectUniquePtr callClassifyMethod(PyObject* args);

	PyObject* createArrayOfFeatures(const FlowFeatures& features) const;
	PyObjectUniquePtr
	createArrayOfBurstsOfFeatures(const std::vector<FlowFeatures>& burstOfFeatures) const;

	std::string m_bridgePath;
	std::string m_mlModelPath;
	PyObjectUniquePtr m_bridgeModule = nullptr;
	PyObjectUniquePtr m_initFunction = nullptr;
	PyObjectUniquePtr m_scikitMlModel = nullptr;
	PyObjectUniquePtr m_classifyFunction = nullptr;
	std::vector<FeatureID> m_featureIDs;
};

} // namespace WIF
