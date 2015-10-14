/**
 * @file		numeric_conversions.hpp
 * Contains numeric conversion functions.
 *
 * @author		Jennifer Yao
 * @date		10/9/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_NUMERIC_CONVERSIONS_HPP
#define CALC_NUMERIC_CONVERSIONS_HPP

#include "config.hpp"

#if HAVE_STD_NUMERIC_CONVERSIONS
#  include <cstdlib>
#  include <cwchar>
#else
#  include <stdlib.h>
#  include <wchar.h>
#endif

#include <cerrno>
#include <limits>
#include <stdexcept>
#include <type_traits>

#if HAVE_STD_STRING_NUMERIC_CONVERSIONS
#  include <string>
#endif

#if HAVE_STD_NUMERIC_CONVERSIONS
#  define STD std
#else
#  define STD
#endif

namespace calc {
#if !HAVE_STD_STRING_NUMERIC_CONVERSIONS
	/**
	 * Helper for all the sto* functions.
	 *
	 * Shamelessly ripped from libstdc++'s ext/string_conversions.h header.
	 *
	 * @tparam TRet		The return type of the numeric conversion function
	 * 					@p convf.
	 * @tparam Ret		The return type of stoa(), which defaults to @p TRet.
	 * @tparam CharT	The character type of the C string to be converted.
	 * @tparam Base		Additional type template parameters specifying the
	 * 					types of variadic arguments to be passed to @p convf.
	 * @param  convf	A pointer to the function to use for numeric
	 * 					conversion.
	 * @param  name		A C string containing the name of the string numeric
	 * 					conversion function.
	 * @param  str		The C string to be converted.
	 * @param  idx		A pointer to the integer that will store the number
	 * 					of characters that were processed by the conversion.
	 * 					May be ignored by passing @c NULL.
	 * @param  base		Additional variadic arguments to be passed to
	 * 					@p convf.
	 * @return			The C string @p str converted to a numeric value of
	 * 					type @p Ret.
	 */
	template <typename TRet, typename Ret = TRet, typename CharT,
	          typename... Base>
	Ret
	stoa(TRet (*convf)(const CharT*, CharT**, Base...), const char* name,
		const CharT* str, std::size_t* idx, Base... base)
	{
		Ret ret;

		CharT* endptr;
		errno = 0;
		const TRet tmp = convf(str, &endptr, base...);

		if (endptr == str)
			throw std::invalid_argument(name);
		else if (errno == ERANGE
		         || (std::is_same<Ret, int>::value
		             && (tmp < std::numeric_limits<int>::min()
		                 || tmp > std::numeric_limits<int>::max())))
			throw std::out_of_range(name);
		else
			ret = tmp;

		if (idx)
			*idx = endptr - str;

		return ret;
	}
#endif

	inline int
	stoi(const std::string& str, std::size_t* idx = 0, int base = 10) {
#if HAVE_STD_STRING_NUMERIC_CONVERSIONS
		return std::stoi(str, idx, base);
#else
		return stoa<long, int>(&STD::strtol, "stoi", str.c_str(), idx, base);
#endif
	}

	inline long long
	stoll(const std::string& str, std::size_t* idx = 0, int base = 10) {
#if HAVE_STD_STRING_NUMERIC_CONVERSIONS
		return std::stoll(str, idx, base);
#else
		return stoa(&STD::strtoll, "stoll", str.c_str(), idx, base);
#endif
	}

	inline int
	stoi(const std::wstring& str, std::size_t* idx = 0, int base = 10) {
#if HAVE_STD_STRING_NUMERIC_CONVERSIONS
		return std::stoi(str, idx, base);
#else
		return stoa<long, int>(&STD::wcstol, "stoi", str.c_str(), idx, base);
#endif
	}

	inline long long
	stoll(const std::wstring& str, std::size_t* idx = 0, int base = 10) {
#if HAVE_STD_STRING_NUMERIC_CONVERSIONS
		return std::stoll(str, idx, base);
#else
		return stoa(&STD::wcstoll, "stoll", str.c_str(), idx, base);
#endif
	}
} // namespace calc

#endif // CALC_NUMERIC_CONVERSIONS_HPP
