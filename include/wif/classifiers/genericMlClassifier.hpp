/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Generic machine learning classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/classifiers/classifier.hpp"

#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Abstract class specifying interfaces for ML classifiers (scikitMlClassifier and
 * mlpackClassifier)
 *
 */
class GenericMlClassifier : public Classifier {
public:
	/**
	 * @brief Return the path of the ML model, which is currently loaded
	 * @return const std::string& path of the model.
	 */
	virtual const std::string& getMlModelPath() const noexcept = 0;

	/**
	 * @brief Reload the model from file, which was set in the constructor
	 */
	virtual void reloadModelFromDisk() = 0;
};

} // namespace WIF
