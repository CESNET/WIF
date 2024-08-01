/**
 * @file
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief IP address interface
 * Based on the https://nemea.liberouter.org/doc/unirec/ipaddr_8h_source.html
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <string>

namespace WIF {

/**
 * @brief Class representing an IP address of both v4 and v6 versions
 */
class IpAddress {
public:
	/**
	 * @brief IpVersion enum defines supported versions of IP address
	 */
	enum class IpVersion { V4, V6 };

	/**
	 * @brief This exception is thrown when construction of IpAddress object from std::string
	 * fails
	 */
	class FormatError : std::exception {};

	/**
	 * @brief Construct an empty IP address object
	 */
	IpAddress();

	/**
	 * @brief Create a new IP address object from int representation
	 * @param data int representation of IPv4 address (in little endian)
	 */
	IpAddress(uint32_t data);

	/**
	 * @brief Create a new IP address from bytes
	 * @param bytes byte representation of IPv4 address (4 bytes for v4, 16 bytes for v6)
	 * @param ipVersion
	 * @param isLittleEndian true if bytes are in Little Endian, false if in Big Endian
	 */
	IpAddress(const uint8_t* bytes, IpVersion ipVersion, bool isLittleEndian = true);

	/**
	 * @brief Create a new IP address object from std::string
	 * @param str string with IPv4 or IPv6 address
	 * @return IpAddress
	 * @throw FormatError if parsing fails
	 */
	IpAddress(const std::string& str);

	/**
	 * @brief Destroy the IP address object
	 */
	~IpAddress() = default;

	/**
	 * @brief Returns whether the IP address is empty
	 * @return bool
	 */
	bool empty() const noexcept;

	/**
	 * Getter for version of the IP address.
	 * @return IpVersion version of the held IP address
	 */
	IpVersion getVersion() const noexcept;

	/**
	 * @brief Checks if the IP address is version 4
	 * @return bool
	 */
	bool isIPv4() const noexcept;

	/**
	 * @brief Checks if the IP address is version 6
	 * @return bool
	 */
	bool isIPv6() const noexcept;

	/**
	 * @brief Getter for IPv4 address as int in little endian
	 * @return uint32_t
	 */
	uint32_t v4AsInt() const noexcept;

	/**
	 * @brief Getter for pointer to underlying storage
	 * @return const uint8_t*
	 */
	const uint8_t* data() const noexcept;

	/**
	 * @brief Getter for IPv4 address as bytes
	 * @return const char*
	 */
	const char* v4AsBytes() const noexcept;

	/**
	 * @brief Getter for IPv6 address as int array
	 * @return const uint32_t*
	 */
	const uint32_t* v6AsIntArray() const noexcept;

	/**
	 * @brief Getter for IPv6 address as bytes
	 * @return const char*
	 */
	const char* v6AsBytes() const noexcept;

	/**
	 * @brief Getter for IP address representation in std::string
	 * @return std::string
	 */
	std::string toString() const;

	/**
	 * @brief Comparison operator ==
	 *
	 * @param l left operand
	 * @param r right operand
	 * @return bool true if IP addresses are the same, false otherwise
	 */
	friend bool operator==(const IpAddress& l, const IpAddress& r);

	/**
	 * @brief Logical AND operator
	 *
	 * @param l left operand
	 * @param r right operand
	 * @return IpAddress
	 */
	friend IpAddress operator&(const IpAddress& l, const IpAddress& r);

	/**
	 * @brief Bitwise NOT operator
	 *
	 * @param address IP address whose bits will be flipped
	 * @return IpAddress IP address with flipped bits
	 */
	friend IpAddress operator~(const IpAddress& address);

	/**
	 * @brief Comparison operator <
	 *
	 * @param l left operand
	 * @param r right operand
	 * @return bool true if l is less than r
	 */
	friend bool operator<(const IpAddress& l, const IpAddress& r);

private:
	constexpr static size_t SIZE_IN_UINT8 = 16;
	constexpr static size_t SIZE_IN_UINT16 = 8;
	constexpr static size_t SIZE_IN_UINT32 = 4;
	constexpr static size_t SIZE_IN_UINT64 = 2;

	union IpData {
		uint8_t ui8[SIZE_IN_UINT8];
		uint16_t ui16[SIZE_IN_UINT16];
		uint32_t ui32[SIZE_IN_UINT32];
		uint64_t ui64[SIZE_IN_UINT64];
	};

	constexpr static uint32_t IPV4_FILLING_CONSTANT = 0xFFFFFFFF;

	bool compareV4(const IpAddress& other) const;
	bool compareV6(const IpAddress& other) const;

	void createV4FromBytes(const uint8_t* bytes, bool isLittleEndian);
	void createV6FromBytes(const uint8_t* bytes, bool isLittleEndian);
	void createV4FromBytesLittleEndian(const uint8_t* bytes);
	void createV4FromBytesBigEndian(const uint8_t* bytes);
	void createV6FromBytesLittleEndian(const uint8_t* bytes);
	void createV6FromBytesBigEndian(const uint8_t* bytes);

	IpData m_ipData;
};

} // namespace WIF
