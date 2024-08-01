/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Classifier interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/combinators/combinator.hpp"
#include "wif/storage/clfResult.hpp"
#include "wif/storage/flowFeatures.hpp"

#include <memory>
#include <vector>

namespace WIF {

/**
 * @brief Classifier class
 *
 * Represents an abstract model which classifies flow or bulk of flows
 */
class Classifier {
public:
	/**
	 * @brief Destroy the Classifier object
	 *
	 */
	virtual ~Classifier() = default;

	/**
	 * @brief Set feature IDs which will be used for classification
	 *
	 * @param sourceFeatureIDs
	 */
	virtual void setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs);

	/**
	 * @brief Perform classification of one flow
	 * Only flowFeatures fields set by setFeatureSourceIDs() are processed
	 *
	 * @param flowFeatures flow features to classify
	 * @return ClfResult classification result
	 */
	virtual ClfResult classify(const FlowFeatures& flowFeatures) = 0;

	/**
	 * @brief Perform classification of burst of flows
	 *
	 * @param burstOfFlowFeatures burst of flow features to classify
	 * @return std::vector<ClfResult> classification results
	 */
	virtual std::vector<ClfResult> classify(const std::vector<FlowFeatures>& burstOfFlowFeatures)
		= 0;

	/**
	 * @brief Getter for source feature IDs
	 *
	 * @return const std::vector<FeatureID>&
	 */
	const std::vector<FeatureID>& getSourceFeatureIDs() const;

private:
	std::vector<FeatureID> m_sourceFeatureIDs;
};

} // namespace WIF
