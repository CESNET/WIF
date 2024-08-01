/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Binary DST combinator interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "combinator.hpp"

#include <dst/Evidence.h>
#include <dst/Universe.h>
#include <vector>

namespace WIF {

/**
 * @brief BinaryDSTCombinator class
 *
 * Provides combination of values by the Dempster-Shafer Theory
 */
class BinaryDSTCombinator : public Combinator {
public:
	/**
	 * @brief Combine multiple values into one by using the DST
	 *
	 * Each value is used to create an evidence with a positive hypothesis with probability of value
	 * and a negative hypothesis with probability 1 - value. All evidences are then combined
	 * together and the resulting probability of the positive hypothesis is returned.
	 *
	 * @param valuesToCombine vector of doubles to combine
	 * @return double belief of the positive hypothesis after DST combination
	 */
	double combine(const std::vector<double>& valuesToCombine) override;
};

} // namespace WIF
