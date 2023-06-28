/**
 * @file
 * @author Pavel Siska <siska@cesnet.cz>
 * @author Richard Plny <plnyrich@fit.cvut.cz>
 * @brief Regex pattern implementation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wif/regex/regexPattern.hpp"

namespace WIF {

RegexPattern::RegexPattern(
	const std::vector<std::string>& regexes,
	PatternMatchMode patternMatchMode)
{
	switch (patternMatchMode) {
	case PatternMatchMode::ALL:
		m_matchFunction = &RegexPattern::matchAll;
		break;
	case PatternMatchMode::PART:
		m_matchFunction = &RegexPattern::matchPart;
		break;
	case PatternMatchMode::ANY:
		m_matchFunction = &RegexPattern::matchAny;
		break;
	}

	if (!regexes.size()) {
		throw std::runtime_error(
			"RegexPattern::RegexPattern() has failed. Got 0 regexes to match!");
	}

	for (const auto& regex : regexes) {
		m_patternsToMatch.emplace_back(boost::regex(regex));
	}
}

double RegexPattern::match(const std::string& str) const
{
	return (this->*m_matchFunction)(str);
}

double RegexPattern::matchAll(const std::string& str) const
{
	for (const auto& regex : m_patternsToMatch) {
		if (!regex_search(str, regex)) {
			return REGEX_PATTERN_NOT_FOUND;
		}
	}

	return REGEX_PATTERN_FOUND;
}

double RegexPattern::matchPart(const std::string& str) const
{
	uint16_t matchedPatterns = 0;
	for (const auto& regex : m_patternsToMatch) {
		if (regex_search(str, regex)) {
			matchedPatterns++;
		}
	}

	if (matchedPatterns == m_patternsToMatch.size()) {
		return REGEX_PATTERN_FOUND;
	}

	return (REGEX_PATTERN_FOUND / m_patternsToMatch.size()) * matchedPatterns;
}

double RegexPattern::matchAny(const std::string& str) const
{
	for (const auto& regex : m_patternsToMatch) {
		if (regex_search(str, regex)) {
			return REGEX_PATTERN_FOUND;
		}
	}

	return REGEX_PATTERN_NOT_FOUND;
}

} // namespace WIF
