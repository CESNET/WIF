/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief IP prefix interface
 * Based on:
 * https://github.com/CESNET/nemea-modules-ng/blob/main/modules/whitelist/src/ipAddressPrefix.hpp
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "wif/storage/ipAddress.hpp"

namespace WIF {

/**
 * @brief Class representing an IP prefix (or an IP subnet)
 */
class IpPrefix {
public:
	/**
	 * @brief Max bit lenth of IPv4 prefix
	 */
	static constexpr size_t IPV4_MAX_PREFIX_LENGTH = 32;

	/**
	 * @brief Max bit lenth of IPv6 prefix
	 */
	static constexpr size_t IPV6_MAX_PREFIX_LENGTH = 128;

	/**
	 * @brief Construct a new IP Prefix object representing just 1 IP address
	 *
	 * @param address single IP address
	 */
	IpPrefix(const IpAddress& address);

	/**
	 * @brief Construct a new IP Prefix object
	 * @param addressStr string representation of IP prefix
	 * @param prefixLength number of bits of the prefix
	 */
	IpPrefix(const std::string& addressStr, size_t prefixLength);

	/**
	 * @brief Construct a new IP Prefix object
	 * @param address IP prefix
	 * @param prefixLength number of bits of the prefix
	 */
	IpPrefix(const IpAddress& address, size_t prefixLength);

	/**
	 * @brief Getter for the number of bits used by prefix
	 * @return size_t number of bits of the prefix
	 */
	size_t prefixLength() const noexcept { return m_prefixLength; }

	/**
	 * @brief Getter for number of IP addresses in the prefix
	 * @return size_t number of IP addresses in the prefix
	 */
	size_t size() const noexcept;

	/**
	 * @brief Getter for the prefix address
	 * @return const IpAddress& prefix address
	 */
	const IpAddress& getPrefix() const noexcept { return m_prefixAddress; }

	/**
	 * @brief Getter for the prefix mask
	 * @return const IpAddress& prefix mask
	 */
	const IpAddress& getMask() const noexcept { return m_prefixMask; }

	/**
	 * @brief Match IP prefix and check if IP address in part of the prefix
	 * @param ipAddress to be matched
	 * @return true if ipAddress is in this prefix
	 * @return false otherwise
	 */
	bool match(const IpAddress& ipAddress) const noexcept;

	/**
	 * @brief Comparison operator <
	 * @param l left operand
	 * @param r right operand
	 * @return bool true if l is less than r
	 */
	friend bool operator<(const IpPrefix& l, const IpPrefix& r);

	/**
	 * @brief Comparison operator <
	 * Used for binary search of IP address in a vector of IP prefixes
	 * @param l left operand
	 * @param r right operand
	 * @return bool true if l is less than r
	 */
	friend bool operator<(const IpAddress& l, const IpPrefix& r);

	/**
	 * @brief Comparison operator <
	 * Used for binary search of IP address in a vector of IP prefixes
	 * @param l left operand
	 * @param r right operand
	 * @return bool true if l is less than r
	 */
	friend bool operator<(const IpPrefix& l, const IpAddress& r);

private:
	IpAddress m_prefixAddress;
	IpAddress m_prefixMask;
	size_t m_prefixLength;
};

} // namespace WIF
