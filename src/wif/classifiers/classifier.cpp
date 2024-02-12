/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Classifier implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/classifiers/classifier.hpp"

namespace WIF {

void Classifier::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	m_sourceFeatureIDs = sourceFeatureIDs;
}

const std::vector<FeatureID>& Classifier::getSourceFeatureIDs() const
{
	return m_sourceFeatureIDs;
}

} // namespace WIF
