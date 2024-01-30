/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Unirec reporter interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/reporters/reporter.hpp"

#include <unirec++/unirec.hpp>

namespace WIF {

/**
 * @brief Unirec Reporter class
 *
 * Reporter specialization which sends data in UniRec format to Trap interface
 */
class UnirecReporter : public Reporter {
public:
	/**
	 * @brief Construct a new Unirec Reporter object
	 *
	 * @param outputInterface output interface where data will be sent
	 */
	UnirecReporter(Nemea::UnirecOutputInterface& outputInterface);

	/**
	 * @brief Update UniRec field IDs
	 * Should be called after UniRec template was changed
	 *
	 * @param unirecFieldIDs
	 */
	void updateUnirecFieldIDs(const std::vector<ur_field_id_t>& unirecFieldIDs);

	/*! @copydoc Reporter::onRecordStart()
	 */
	virtual void onRecordStart() override;

	/*! @copydoc Reporter::report(const DataVariant& data)
	 * Data is stored to the UniRec record to the next ID
	 */
	void report(const DataVariant& data) override;

	/*! @copydoc Reporter::onRecordEnd()
	 * Current Unirec Record is sent to Unirec Output Interface
	 */
	virtual void onRecordEnd() override;

	/*! @copydoc Reporter::flush()
	 * Internally calls Nemea::UnirecOutputInterface::sendFlush()
	 */
	virtual void flush() override;

private:
	unsigned m_currentUnirecID = 0;
	std::vector<ur_field_id_t> m_unirecFieldIDs;
	Nemea::UnirecOutputInterface& m_outputInterface;
};

} // namespace WIF
