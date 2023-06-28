/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Binary DST combinator implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/combinators/binaryDSTCombinator.hpp"

namespace WIF {

double BinaryDSTCombinator::combine(const std::vector<double>& valuesToCombine) const
{
	if (valuesToCombine.size() == 0) {
		return 0.0;
	}

	constexpr size_t POSITIVE_HYPOTHESIS = 0xAA;
	constexpr size_t NEGATIVE_HYPOTHESIS = 0xBB;

	CPPDST::CUniverse dstUniverse;
	dstUniverse.addHypotheseses(POSITIVE_HYPOTHESIS);
	dstUniverse.addHypotheseses(NEGATIVE_HYPOTHESIS);

	CPPDST::CEvidence combinedEvidence = dstUniverse.addEvidence();
	for (size_t valueIdx = 0; valueIdx < valuesToCombine.size(); ++valueIdx) {
		auto newEvidence = dstUniverse.addEvidence();
		newEvidence.addFocalSet(valuesToCombine[valueIdx], POSITIVE_HYPOTHESIS);
		newEvidence.addFocalSet(1 - valuesToCombine[valueIdx], NEGATIVE_HYPOTHESIS);

		combinedEvidence = (valueIdx == 0) ? newEvidence : combinedEvidence & newEvidence;
	}

	return combinedEvidence.belief(POSITIVE_HYPOTHESIS);
}

} // namespace WIF
