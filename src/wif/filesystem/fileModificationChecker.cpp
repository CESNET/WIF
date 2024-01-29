/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief File modification checker implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/filesystem/fileModificationChecker.hpp"

namespace WIF {

FileModificationChecker::FileModificationChecker(const std::string& filePath)
	: m_filePath(filePath)
{
	m_lastModifiedTime = getLastModifiedTime();
}

bool FileModificationChecker::isChangeDetected()
{
	auto lastModifiedTime = getLastModifiedTime();
	if (lastModifiedTime > m_lastModifiedTime) {
		m_lastModifiedTime = lastModifiedTime;
		return true;
	}
	return false;
}

std::filesystem::file_time_type FileModificationChecker::getLastModifiedTime() const
{
	return std::filesystem::last_write_time(m_filePath);
}

} // namespace WIF
