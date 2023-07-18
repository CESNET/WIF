/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Aggregation record
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <climits>
#include <cstddef>
#include <cstdint>
#include <ostream>

namespace WIF::Aggregation {

/**
 * @brief Aggregation record class
 *
 * @tparam T underlying data type
 */
template<typename T>
class AggregationRecord {
public:
	/**
	 * @brief Construct a new Aggregation Record object
	 *
	 */
	AggregationRecord(T value = 0x00)
		: m_value(value)
	{
	}

	/**
	 * @brief Destroy the Aggregation Record object
	 *
	 */
	~AggregationRecord() = default;

	/**
	 * @brief Getter for current held value
	 *
	 * @return T copy of held value
	 */
	T value() const noexcept { return m_value; }

	/**
	 * @brief Clear held value
	 *
	 */
	void clear() noexcept { m_value = 0x00; }

	/**
	 * @brief Increase the held value by one
	 *
	 */
	void increase() noexcept { ++m_value; }

	/**
	 * @brief Decrease the held value by one
	 *
	 */
	void decrease() noexcept { --m_value; }

	/**
	 * @brief Set N-th bit to one
	 *
	 * @param index index of the bit to set
	 */
	void setTrue(size_t index) noexcept { m_value |= (0x01 << index); }

	/**
	 * @brief Set N-th bit to zero
	 *
	 * @param index index of the bit to set
	 */
	void setFalse(size_t index) noexcept { m_value &= ~(0x01 << index); }

	/**
	 * @brief Perform logical AND of the held and argument values store result as the new held value
	 *
	 * @param value
	 */
	void andValue(T value) noexcept { m_value &= value; }

	/**
	 * @brief Perform logical OR of the held and argument values store result as the new held value
	 *
	 * @param value
	 */
	void orValue(T value) noexcept { m_value |= value; };

	/**
	 * @brief Compare values and decide whenever held values are the same
	 *
	 * @param other record to be be compared with
	 * @return true if held values are same, false otherwise
	 */
	bool hasSameValue(const AggregationRecord<T>& other) const noexcept
	{
		return m_value == other.m_value;
	}

	/**
	 * @brief Getter for specified bit
	 *
	 * @param index index of the bit
	 * @return uint8_t value of the bit (0/1)
	 */
	uint8_t operator[](size_t index) const { return (m_value >> index) & 0x01; }

	/**
	 * @brief Getter for capacity.
	 *
	 * Aggregation record (with integer underlying type) can be used for storing True/False values.
	 * For example, if an event was seen or not. This is done by bit masking into the underlying
	 * type. Therefore, capacity of the Aggregation record (number of bytes available for bit
	 * masking) is size of the underlying type times 8 bits in each byte.
	 *
	 * @return size_t
	 */
	static constexpr size_t capacity() noexcept { return CHAR_BIT * sizeof(T); }

	friend std::ostream& operator<<(std::ostream& os, const AggregationRecord& record)
	{
		T val = record.value();
		size_t total = capacity();
		for (size_t i = 0; i < total; ++i) {
			os << (int) ((val >> (total - i - 1)) & 0x01);
		}
		return os;
	}

private:
	T m_value = 0;
};

template class AggregationRecord<uint8_t>;
template class AggregationRecord<uint16_t>;
template class AggregationRecord<uint32_t>;
template class AggregationRecord<uint64_t>;

} // namespace WIF::Aggregation
