/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Sum combinator implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/combinators/sumCombinator.hpp"

#include <numeric>
#include <stdexcept>

namespace WIF {

double SumCombinator::combine(const std::vector<double>& valuesToCombine)
{
	if (valuesToCombine.empty()) {
		throw std::runtime_error("Nothing to combine! The supplied vector is empty.");
	}

	return std::accumulate(valuesToCombine.begin(), valuesToCombine.end(), 0.0);
}

} // namespace WIF
