/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Average combinator interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "combinator.hpp"

#include <vector>

namespace WIF {

/**
 * @brief AverageCombinator class
 *
 * Provides average value of input data
 */
class AverageCombinator : public Combinator {
public:
	/**
	 * @brief Combine multiple values into one by using average function
	 *
	 * @param valuesToCombine vector of doubles to combine
	 * @return double average of values in the input vector
	 */
	double combine(const std::vector<double>& valuesToCombine) const override;
};

} // namespace WIF
