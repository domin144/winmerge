/**
 * @file  FilterList.h
 *
 * @brief Declaration file for FilterList.
 */
#pragma once

#include <vector>
#include <memory>
#include <Poco/RegularExpression.h>
#include "unicoder.h"

/**
 * @brief Container for one filtering rule / compiled expression.
 * This structure holds compiled regular expression and a original expression
 * as a string. We need the original expression string in case we want to
 * know which regular expression did match.
 */
struct filter_item
{
	std::string filterAsString; /** Original regular expression string */
	Poco::RegularExpression regexp; /**< Compiled regular expression */
	filter_item(const std::string &filter, int reOpts) : filterAsString(filter), regexp(filter, reOpts) {}
};

typedef std::shared_ptr<filter_item> filter_item_ptr;

/**
 * @brief Regular expression list.
 * This class holds a list of regular expressions for matching strings.
 * The class also provides simple function for matching and remembers the
 * last matched expression.
 */
class FilterList
{
public:
	FilterList();
	~FilterList();
	
	void AddRegExp(const std::string& regularExpression);
	void RemoveAllFilters();
	bool HasRegExps() const;
	size_t GetCount() const { return m_list.size(); }
	bool Match(const std::string& string, int codepage = ucr::CP_UTF_8);
	const char * GetLastMatchExpression() const;
	const filter_item& operator[](int index) const;

private:
	std::vector <filter_item_ptr> m_list;
	const std::string *m_lastMatchExpression;

};

struct IgnoredSusbstitutionItem
{
	std::string Tokens[2];
	/// Both tokens are broken down into the three parts:
	///		Tokens[0] = CommonPrefix + MiddleParts[0] + CommonSuffix
	///		Tokens[1] = CommonPrefix + MiddleParts[1] + CommonSuffix
	std::string CommonPrefix;
	std::string MiddleParts[2];
	std::string CommonSuffix;
	size_t CommonPrefixLength; /// For convenience
	size_t CommonSuffixLength;
	Poco::RegularExpression ChangedPartRegexp[2]; /**< Compiled regular expression */

	IgnoredSusbstitutionItem
	(
		const std::string& token0, const std::string& token1,
		int regexpCompileOptions, bool extractCommonSufixAndPrefix
	);
};

class IgnoredSubstitutionsFilterList
{
public:
	IgnoredSubstitutionsFilterList();
	~IgnoredSubstitutionsFilterList();

	void Add(const std::string& change0, const std::string& change1, bool extractCommonSufixAndPrefix);
	void RemoveAllFilters();
	bool HasRegExps() const;
	size_t GetCount() const { return m_list.size(); }
	bool MatchBoth
	(
		size_t filterIndex,
		const std::string& string0,
		const std::string& string1,
		int codepage = CP_UTF8
	) const;
	const IgnoredSusbstitutionItem &operator[](int index) const;

private:
	std::vector<std::shared_ptr<IgnoredSusbstitutionItem>> m_list;
};

/** 
 * @brief Removes all expressions from the list.
 */
inline void FilterList::RemoveAllFilters()
{
	m_list.clear();
}

/** 
 * @brief Returns if list has any expressions.
 * @return true if list contains one or more expressions.
 */
inline bool FilterList::HasRegExps() const
{
	return !m_list.empty();
}

/** 
 * @brief Returns the last matched expression (if any).
 * This function returns the regular expression string that matched last.
 * @return Last matched expression, or `nullptr` in case no matches yet.
 */
inline const char * FilterList::GetLastMatchExpression() const
{
	return m_lastMatchExpression->c_str();
}

inline const filter_item& FilterList::operator[](int index) const
{
	const filter_item_ptr &item = m_list[index];
	return *item;
}

