/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief IP address implementation
 * Based on the https://nemea.liberouter.org/doc/unirec/ipaddr_8h_source.html
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/storage/ipAddress.hpp"

namespace WIF {

IpAddress::IpAddress()
{
	m_ipData.ui64[0] = 0;
	m_ipData.ui64[1] = 0;
}

IpAddress::IpAddress(uint32_t data)
{
	m_ipData.ui64[0] = 0;
	m_ipData.ui32[2] = htonl(data);
	m_ipData.ui32[3] = IPV4_FILLING_CONSTANT;
}

IpAddress::IpAddress(const uint8_t* bytes, IpVersion ipVersion, bool isLittleEndian)
{
	if (ipVersion == IpVersion::V4) {
		createV4FromBytes(bytes, isLittleEndian);
	} else if (ipVersion == IpVersion::V6) {
		createV6FromBytes(bytes, isLittleEndian);
	} else {
		throw std::runtime_error("IpAddress::IpAddress(): Unknown IpVersion!");
	}
}

IpAddress::IpAddress(const std::string& str)
{
	uint8_t tmp[16];
	// : is only in IPv6
	// Not Found -> IPv4, Found -> IPv6
	if (str.find(':') == std::string::npos) {
		if (inet_pton(AF_INET, str.data(), (void*) tmp) != 1) {
			throw FormatError();
		}
		createV4FromBytesLittleEndian(tmp);
	} else {
		if (inet_pton(AF_INET6, str.data(), (void*) tmp) != 1) {
			throw FormatError();
		}
		createV6FromBytesLittleEndian(tmp);
	}
}

bool IpAddress::empty() const noexcept
{
	if (m_ipData.ui64[0] == 0) {
		if (m_ipData.ui64[1] == 0
			|| (m_ipData.ui32[2] == 0 && m_ipData.ui32[3] == IPV4_FILLING_CONSTANT)) {
			return true;
		}
	}
	return false;
}

IpAddress::IpVersion IpAddress::getVersion() const noexcept
{
	return isIPv4() ? IpVersion::V4 : IpVersion::V6;
}

bool IpAddress::isIPv4() const noexcept
{
	return (m_ipData.ui64[0] == 0 && m_ipData.ui32[3] == IPV4_FILLING_CONSTANT);
}

bool IpAddress::isIPv6() const noexcept
{
	return !isIPv4();
}

uint32_t IpAddress::v4AsInt() const noexcept
{
	return ntohl(m_ipData.ui32[2]);
}

const uint8_t* IpAddress::data() const noexcept
{
	return &m_ipData.ui8[0];
}

const char* IpAddress::v4AsBytes() const noexcept
{
	return reinterpret_cast<const char*>(&m_ipData.ui8[8]);
}

const uint32_t* IpAddress::v6AsIntArray() const noexcept
{
	return reinterpret_cast<const uint32_t*>(&m_ipData.ui32);
}

const char* IpAddress::v6AsBytes() const noexcept
{
	return reinterpret_cast<const char*>(&m_ipData.ui8);
}

std::string IpAddress::toString() const
{
	char buffer[64];
	if (isIPv4()) {
		inet_ntop(AF_INET, v4AsBytes(), buffer, INET6_ADDRSTRLEN);
	} else {
		inet_ntop(AF_INET6, v6AsBytes(), buffer, INET6_ADDRSTRLEN);
	}
	return std::string(buffer, std::strlen(buffer));
}

bool operator==(const IpAddress& l, const IpAddress& r)
{
	return std::memcmp(l.m_ipData.ui8, r.m_ipData.ui8, IpAddress::SIZE_IN_UINT8) == 0;
}

IpAddress operator&(const IpAddress& l, const IpAddress& r)
{
	IpAddress result;
	for (unsigned ui64Idx = 0; ui64Idx < IpAddress::SIZE_IN_UINT64; ++ui64Idx) {
		result.m_ipData.ui64[ui64Idx] = l.m_ipData.ui64[ui64Idx] & r.m_ipData.ui64[ui64Idx];
	}
	return result;
}

void IpAddress::createV4FromBytes(const uint8_t* bytes, bool isLittleEndian)
{
	if (isLittleEndian) {
		createV4FromBytesLittleEndian(bytes);
	} else {
		createV4FromBytesBigEndian(bytes);
	}
}

void IpAddress::createV6FromBytes(const uint8_t* bytes, bool isLittleEndian)
{
	if (isLittleEndian) {
		createV6FromBytesLittleEndian(bytes);
	} else {
		createV6FromBytesBigEndian(bytes);
	}
}

void IpAddress::createV4FromBytesLittleEndian(const uint8_t* bytes)
{
	m_ipData.ui64[0] = 0;
	m_ipData.ui8[8] = bytes[0];
	m_ipData.ui8[9] = bytes[1];
	m_ipData.ui8[10] = bytes[2];
	m_ipData.ui8[11] = bytes[3];
	m_ipData.ui32[3] = IPV4_FILLING_CONSTANT;
}

void IpAddress::createV4FromBytesBigEndian(const uint8_t* bytes)
{
	m_ipData.ui64[0] = 0;
	m_ipData.ui8[8] = bytes[3];
	m_ipData.ui8[9] = bytes[2];
	m_ipData.ui8[10] = bytes[1];
	m_ipData.ui8[11] = bytes[0];
	m_ipData.ui32[3] = IPV4_FILLING_CONSTANT;
}

void IpAddress::createV6FromBytesLittleEndian(const uint8_t* bytes)
{
	std::memcpy(&m_ipData, bytes, IpAddress::SIZE_IN_UINT8);
}

void IpAddress::createV6FromBytesBigEndian(const uint8_t* bytes)
{
	for (unsigned byteIdx = 0; byteIdx < IpAddress::SIZE_IN_UINT8; ++byteIdx) {
		m_ipData.ui8[byteIdx] = bytes[IpAddress::SIZE_IN_UINT8 - 1 - byteIdx];
	}
}

} // namespace WIF
