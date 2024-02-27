/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief IP prefix implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/utils/ipPrefix.hpp"

#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>

namespace {

void validatePrefixLength(size_t prefixLength, size_t maxPrefixLength)
{
	if (prefixLength > maxPrefixLength) {
		throw std::invalid_argument(
			"IP prefix is too long. Given: " + std::to_string(prefixLength)
			+ ", max: " + std::to_string(maxPrefixLength));
	}
}

WIF::IpAddress createIpV6MaskFromPrefix(size_t prefixLength)
{
	uint8_t prefixBytes = prefixLength / 8;
	uint8_t prefixBits = prefixLength % 8;
	uint8_t data[16];

	for (unsigned bytesIndex = 0; bytesIndex < prefixBytes; ++bytesIndex) {
		data[bytesIndex] = UINT8_MAX;
	}

	if (prefixBits != 0U) {
		data[prefixBytes] = UINT8_MAX << (CHAR_BIT - prefixBits);
	}

	return WIF::IpAddress(data, WIF::IpAddress::IpVersion::V6);
}

} // namespace

namespace WIF {

IpPrefix::IpPrefix(const IpAddress& address)
	: IpPrefix(address, address.isIPv4() ? IPV4_MAX_PREFIX_LENGTH : IPV6_MAX_PREFIX_LENGTH)
{
}

IpPrefix::IpPrefix(const std::string& addressStr, size_t prefixLength)
	: IpPrefix(IpAddress(addressStr), prefixLength)
{
}

IpPrefix::IpPrefix(const IpAddress& address, size_t prefixLength)
	: m_prefixLength(prefixLength)
{
	if (address.isIPv4()) {
		validatePrefixLength(prefixLength, IPV4_MAX_PREFIX_LENGTH);
		size_t shift = IPV4_MAX_PREFIX_LENGTH - prefixLength;
		m_prefixMask = IpAddress(std::numeric_limits<uint32_t>::max() << shift);
	} else {
		validatePrefixLength(prefixLength, IPV6_MAX_PREFIX_LENGTH);
		m_prefixMask = createIpV6MaskFromPrefix(prefixLength);
	}
	m_prefixAddress = address & m_prefixMask;
}

size_t IpPrefix::size() const noexcept
{
	if (m_prefixAddress.isIPv4()) {
		return std::pow(2, IPV4_MAX_PREFIX_LENGTH - m_prefixLength);
	} else {
		return std::pow(2, IPV6_MAX_PREFIX_LENGTH - m_prefixLength);
	}
}

bool IpPrefix::match(const IpAddress& ipAddress) const noexcept
{
	if (ipAddress.getVersion() != m_prefixAddress.getVersion()) {
		return false;
	}
	return (ipAddress & m_prefixMask) == m_prefixAddress;
}

bool operator<(const IpPrefix& l, const IpPrefix& r)
{
	// IPv4 is before IPv6
	if (l.m_prefixAddress.isIPv4() && r.m_prefixAddress.isIPv6()) {
		return true;
	} else if (l.m_prefixAddress.isIPv6() && r.m_prefixAddress.isIPv4()) {
		return false;
	} // Now we are sure that both prefixes have the same IP version

	if (l.m_prefixAddress == r.m_prefixAddress) {
		return l.m_prefixLength < r.m_prefixLength;
	}
	return l.m_prefixAddress < r.m_prefixAddress;
}

bool operator<(const IpAddress& l, const IpPrefix& r)
{
	return l < r.m_prefixAddress;
}

bool operator<(const IpPrefix& l, const IpAddress& r)
{
	return l.m_prefixAddress < r;
}

} // namespace WIF
