/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Aggregation table interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/aggregation/aggregationRecord.hpp"

#include <memory>
#include <vector>

namespace WIF::Aggregation {

/**
 * @brief Aggregation table class
 *
 * @tparam T underlying data type for held aggregation records
 */
template<typename T>
class AggregationTable {
public:
	/**
	 * @brief Construct a new Aggregation table object
	 *
	 * @param size number of records
	 */
	AggregationTable(size_t size)
		: m_table(size, AggregationRecord<T>())
	{
	}

	/**
	 * @brief Destroy the Aggregation table object
	 *
	 */
	virtual ~AggregationTable() = default;

	/**
	 * @brief Getter for aggregation record
	 *
	 * @param index index of aggregation record
	 * @return AggregationRecord<T>&
	 */
	AggregationRecord<T>& operator[](size_t index) { return m_table.at(index); }

	/**
	 * @brief Getter for aggregation record
	 *
	 * @param index index of aggregation record
	 * @return const AggregationRecord<T>&
	 */
	const AggregationRecord<T>& operator[](size_t index) const { return m_table.at(index); }

	/**
	 * @brief Clear the aggregation table
	 *
	 */
	void clear()
	{
		for (size_t recordIdx = 0; recordIdx < m_table.size(); ++recordIdx) {
			m_table[recordIdx].clear();
		}
	}

	/**
	 * @brief Size of the aggregation table
	 *
	 * @return size_t
	 */
	size_t size() const { return m_table.size(); }

private:
	std::vector<AggregationRecord<T>> m_table;
};

template class AggregationTable<uint8_t>;
template class AggregationTable<uint16_t>;
template class AggregationTable<uint32_t>;
template class AggregationTable<uint64_t>;

} // namespace WIF::Aggregation
