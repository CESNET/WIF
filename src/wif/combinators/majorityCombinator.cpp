/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Majority combinator implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/combinators/majorityCombinator.hpp"

#include <numeric>
#include <stdexcept>

namespace WIF {

static double findPrevalentElement(std::vector<double> values)
{
	std::sort(values.begin(), values.end());

	unsigned maxCount = 1;
	unsigned currentCount = 1;
	double candidate = values[0];

	for (unsigned index = 1; index < values.size(); ++index) {
		if (values[index] == values[index - 1]) {
			++currentCount;
		} else {
			currentCount = 1;
		}

		if (currentCount > maxCount) {
			maxCount = currentCount;
			candidate = values[index - 1];
		}
	}

	return candidate;
}

double MajorityCombinator::combine(const std::vector<double>& valuesToCombine)
{
	if (valuesToCombine.empty()) {
		throw std::runtime_error("Nothing to combine! The supplied vector is empty.");
	}

	return findPrevalentElement(valuesToCombine);
}

} // namespace WIF
