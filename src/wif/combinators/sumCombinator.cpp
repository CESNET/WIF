/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Sum combinator implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/combinators/sumCombinator.hpp"

#include <numeric>

namespace WIF {

double SumCombinator::combine(const std::vector<double>& valuesToCombine) const
{
	return std::accumulate(valuesToCombine.begin(), valuesToCombine.end(), 0.0);
}

} // namespace WIF
