/**
 * @file
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief File modification checker interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <filesystem>
#include <string>

namespace WIF {

/**
 * @brief FileModificationChecker class
 *
 * Represents a File on disk
 */
class FileModificationChecker {
public:
	/**
	 * @brief Construct a new FileModificationChecker object
	 * Current last modified time is registered as a latest change
	 *
	 * @param filePath
	 */
	FileModificationChecker(const std::string& filePath);

	/**
	 * @brief Detects change of last modified time of file
	 *
	 * @return true if last modified time is newer when this method was called the last time
	 * @return false if the last modified time remained the same
	 */
	bool isChangeDetected();

	/**
	 * @brief Get the Last Modified Time object
	 *
	 * @return std::filesystem::file_time_type
	 */
	std::filesystem::file_time_type getLastModifiedTime() const;

private:
	std::string m_filePath;
	std::filesystem::file_time_type m_lastModifiedTime;
};

} // namespace WIF
