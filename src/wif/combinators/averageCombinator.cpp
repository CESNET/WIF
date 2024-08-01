/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Average combinator implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/combinators/averageCombinator.hpp"

#include <numeric>
#include <stdexcept>

namespace WIF {

double AverageCombinator::combine(const std::vector<double>& valuesToCombine)
{
	if (valuesToCombine.empty()) {
		throw std::runtime_error("Nothing to combine! The supplied vector is empty.");
	}

	double valuesSum = std::accumulate(valuesToCombine.begin(), valuesToCombine.end(), 0.0);
	return valuesSum / valuesToCombine.size();
}

} // namespace WIF
