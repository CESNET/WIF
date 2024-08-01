/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Reporter
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/dataVariant.hpp"

#include <vector>

namespace WIF {

/**
 * @brief Reporter class
 *
 * Represents an abstract model for reporting (exporting) data and information
 */
class Reporter {
public:
	/**
	 * @brief Destroy the Reporter object
	 *
	 */
	virtual ~Reporter() = default;

	/**
	 * @brief Method, which instructs the reporter to start a new record
	 * All data received by report(...) methods will be stored to the current record
	 *
	 */
	virtual void onRecordStart() = 0;

	/**
	 * @brief Method to report data to the reporter
	 * @param data
	 */
	virtual void report(const DataVariant& data) = 0;

	/**
	 * @brief Method, which tells the reporter to prepare current record for send and then send it
	 * Record can be also stored in queue / bulk for sending out (rather than sending it out at the
	 * moment). If the record needs to be exported right away, use flush()
	 *
	 */
	virtual void onRecordEnd() = 0;

	/**
	 * @brief Forces reporter to send right now all records in queue / bulk (if used)
	 * If reporter does not use queue or bulk and sends each record during onRecordEnd(), this
	 * method does nothing
	 */
	virtual void flush() = 0;
};

} // namespace WIF
