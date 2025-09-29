/**
 * @file
 * @author Jachym Hudlicky <hudlijac@fit.cvut.cz>
 * @brief Abstract Mlpack model implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/ml/mlpackModels/mlpackModel.hpp"

namespace WIF::MlpackModels {

bool MlpackModel::isLoaded() const
{
	return m_loaded;
}

const std::string& MlpackModel::getPath() const
{
	return m_modelPath;
}

void MlpackModel::setFeatureSourceIDs(const std::vector<FeatureID>& sourceFeatureIDs)
{
	m_featureIDs = sourceFeatureIDs;
}

void MlpackModel::convertFeaturesToVector(const FlowFeatures& flowFeatures, arma::vec& vect) const
{
	unsigned index = 0;

	for (const auto& featureID : m_featureIDs) {
		double value = flowFeatures.get<double>(featureID);

		vect(index) = value;
		++index;
	}
}

void MlpackModel::convertBurstOfFeaturesToMatrix(
	const std::vector<FlowFeatures>& burstOfFeatures,
	arma::mat& matrix) const
{
	unsigned row = 0;
	unsigned column = 0;

	for (const auto& feature : burstOfFeatures) {
		row = 0;
		for (const auto& featureId : m_featureIDs) {
			double value = feature.get<double>(featureId);

			matrix(row, column) = value;
			++row;
		}
		++column;
	}
}

} // namespace WIF::MlpackModels
