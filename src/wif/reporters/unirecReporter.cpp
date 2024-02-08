/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Unirec reporter implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/reporters/unirecReporter.hpp"

namespace WIF {

UnirecReporter::UnirecReporter(Nemea::UnirecOutputInterface& outputInterface)
	: m_outputInterface(outputInterface)
{
}

void UnirecReporter::updateUnirecFieldIDs(const std::vector<ur_field_id_t>& unirecFieldIDs)
{
	m_unirecFieldIDs = unirecFieldIDs;
}

void UnirecReporter::onRecordStart()
{
	m_currentUnirecID = 0;
}

void UnirecReporter::report(const DataVariant& data)
{
	ur_field_id_t fieldID = m_unirecFieldIDs[m_currentUnirecID++];
	auto& record = m_outputInterface.getUnirecRecord();

	if (std::holds_alternative<uint8_t>(data)) {
		auto value = std::get<uint8_t>(data);
		record.setFieldFromType<uint8_t>(value, fieldID);
	} else if (std::holds_alternative<uint16_t>(data)) {
		auto value = std::get<uint16_t>(data);
		record.setFieldFromType<uint16_t>(value, fieldID);
	} else if (std::holds_alternative<uint32_t>(data)) {
		auto value = std::get<uint32_t>(data);
		record.setFieldFromType<uint32_t>(value, fieldID);
	} else if (std::holds_alternative<uint64_t>(data)) {
		auto value = std::get<uint64_t>(data);
		record.setFieldFromType<uint64_t>(value, fieldID);
	} else if (std::holds_alternative<double>(data)) {
		auto value = std::get<double>(data);
		record.setFieldFromType<double>(value, fieldID);
	} else if (std::holds_alternative<std::string>(data)) {
		auto value = std::get<std::string>(data);
		record.setFieldFromType<std::string>(value, fieldID);
	} else if (std::holds_alternative<IpAddress>(data)) {
		auto value = std::get<IpAddress>(data);
		Nemea::IpAddress nemeaIp(value.toString());
		record.setFieldFromType<Nemea::IpAddress>(nemeaIp, fieldID);
	} else if (std::holds_alternative<std::vector<double>>(data)) {
		auto value = std::get<std::vector<double>>(data);
		record.setFieldFromVector<double>(value, fieldID);
	}
}

void UnirecReporter::onRecordEnd()
{
	m_outputInterface.send(m_outputInterface.getUnirecRecord());
}

void UnirecReporter::flush()
{
	m_outputInterface.sendFlush();
}

} // namespace WIF
