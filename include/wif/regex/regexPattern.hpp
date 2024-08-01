/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <richard.plny@cesnet.cz>
 * @brief Regex pattern interface
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <boost/regex.hpp>
#include <string>
#include <vector>

namespace WIF {

/**
 * @brief RegexPattern class
 *
 * Class for regular expressions (regex) pattern matching with several operational modes
 */
class RegexPattern {
public:
	static constexpr double REGEX_PATTERN_FOUND = 100;
	static constexpr double REGEX_PATTERN_NOT_FOUND = 0;

	/**
	 * @brief Operational mode
	 *
	 * Influences RegexPattern match output
	 */
	enum class PatternMatchMode {
		ALL, /**< REGEX_PATTERN_FOUND only if ALL regex strings are matched, otherwise
				REGEX_PATTERN_NOT_FOUND */
		PART, /**< Percentage of matched regex strings */
		ANY, /**< REGEX_PATTERN_FOUND if at least one regex string is matched, otherwise
				REGEX_PATTERN_NOT_FOUND */
	};

	/**
	 * @brief Construct a new Regex Pattern object
	 *
	 * @throw std::runtime_error if the vector of regexes is empty
	 *
	 * @param regexes strings with regex patterns to match
	 * @param patternMatchMode
	 */
	RegexPattern(const std::vector<std::string>& regexes, PatternMatchMode patternMatchMode);

	/**
	 * @brief Perform pattern matching in the str, based on the operational mode
	 *
	 * @param str
	 * @return double matching result in percentages, between 0 and 100
	 */
	double match(const std::string& str) const;

private:
	double matchAll(const std::string& str) const;
	double matchPart(const std::string& str) const;
	double matchAny(const std::string& str) const;

	using MatchFunction = double (RegexPattern::*)(const std::string&) const;

	MatchFunction m_matchFunction;
	std::vector<boost::regex> m_patternsToMatch;
};

} // namespace WIF
