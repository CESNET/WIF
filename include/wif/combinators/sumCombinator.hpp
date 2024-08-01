/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Sum combinator interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "combinator.hpp"

#include <vector>

namespace WIF {

/**
 * @brief SumCombinator class
 *
 * Summarize the input data
 */
class SumCombinator : public Combinator {
public:
	/**
	 * @brief Combine multiple values into one by using summarize operation
	 *
	 * @param valuesToCombine vector of doubles to combine
	 * @return double sum of values in the input vector
	 */
	double combine(const std::vector<double>& valuesToCombine) override;
};

} // namespace WIF
