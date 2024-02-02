/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Data variant
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/ipAddress.hpp"

#include <variant>
#include <vector>

namespace WIF {

using DataVariant = std::variant<
	std::monostate,
	uint8_t,
	uint16_t,
	uint32_t,
	uint64_t,
	double,
	std::string,
	IpAddress,
	std::vector<double>>;

} // namespace WIF
