/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Flow features implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/storage/flowFeatures.hpp"

namespace WIF {

FlowFeatures::FlowFeatures(size_t expectedFeatureCount)
{
	m_features.resize(expectedFeatureCount);
}

size_t FlowFeatures::size() const
{
	return m_features.size();
}

void FlowFeatures::clear()
{
	m_features.clear();
}

} // namespace WIF
