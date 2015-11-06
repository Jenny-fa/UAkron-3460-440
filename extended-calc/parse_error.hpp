/**
 * @file		parse_error.hpp
 * Contains type and function declarations for parsing errors.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_PARSE_ERROR_HPP
#define CALC_PARSE_ERROR_HPP

#include "config.hpp"

#include <stdexcept>

#include "script.hpp"

namespace calc {
	template <typename CharT, class Traits = symbol_traits<CharT>>
	class basic_parse_error;

	/// A parsing error for @c char characters.
	typedef basic_parse_error<char> parse_error;

	/// A parsing error for @c wchar_t characters.
	typedef basic_parse_error<wchar_t> wparse_error;

	/**
	 * Defines the type of exception object thrown to report parsing errors.
	 * @tparam CharT	The character type.
	 * @tparam Traits	The symbol traits type.
	 */
	template <typename CharT, class Traits>
	class basic_parse_error : public std::runtime_error {
	public:
		typedef CharT char_type;
		typedef Traits traits_type;
		typedef basic_script_extent<CharT, Traits> extent_type;

		basic_parse_error(error_id code,
		                  const extent_type& extent,
		                  const char* what) :
			std::runtime_error(what), _code(code), _extent(extent)
		{}

		basic_parse_error(error_id code,
		                  const extent_type& extent,
		                  const std::string& what) :
			std::runtime_error(what), _code(code), _extent(extent)
		{}

		constexpr error_id code() const noexcept {
			return this->_code;
		}

		constexpr extent_type extent() const noexcept {
			return this->_extent;
		}

	private:
		error_id _code;
		extent_type _extent;
	};
} // namespace calc

#include "parse_error.ipp"

#endif // CALC_PARSE_ERROR_HPP
