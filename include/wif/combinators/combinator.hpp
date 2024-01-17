/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Combinator interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <vector>

namespace WIF {

/**
 * @brief Combinator class
 *
 * Represents an abstract model which combines multiple values into one
 * Each specialized combinator extends this class
 */
class Combinator {
public:
	/**
	 * Virtual destructor
	 */
	virtual ~Combinator() = default;

	/**
	 * @brief Combine multiple values into one
	 *
	 * @param valuesToCombine vector of doubles to combine
	 * @return double combined double value
	 */
	virtual double combine(const std::vector<double>& valuesToCombine) = 0;
};

} // namespace WIF
