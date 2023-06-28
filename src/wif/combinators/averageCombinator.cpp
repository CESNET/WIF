/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Average combinator implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/combinators/averageCombinator.hpp"

#include <numeric>

namespace WIF {

double AverageCombinator::combine(const std::vector<double>& valuesToCombine) const
{
	double valuesSum = std::accumulate(valuesToCombine.begin(), valuesToCombine.end(), 0.0);
	return valuesToCombine.size() ? valuesSum / valuesToCombine.size() : 0;
}

} // namespace WIF
