/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Majority combinator interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "combinator.hpp"

#include <algorithm>

namespace WIF {

/**
 * @brief MajorityCombinator class
 *
 * Perform majority vote by finding the prevalent element
 */
class MajorityCombinator : public Combinator {
public:
	/**
	 * @brief Combine multiple values into one by using majority
	 * The result is the first prevailing element.
	 *
	 * @throw std::runtime_error if an empty vector is passed
	 * @param valuesToCombine vector of doubles to combine
	 * @return double majority value in the provided vector
	 */
	double combine(const std::vector<double>& valuesToCombine) override;
};

} // namespace WIF
