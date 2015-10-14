/**
 * @file		parser_fwd.hpp
 * Contains forward declarations for the basic_parser class.
 *
 * @author		Jennifer Yao
 * @date		10/9/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_PARSER_FWD_HPP
#define CALC_PARSER_FWD_HPP

namespace calc {
	template <typename CharT> class symbol_traits;
	template <> class symbol_traits<char>;
	template <> class symbol_traits<wchar_t>;

	template <typename CharT, class Traits = symbol_traits<CharT> >
	class basic_parser;

	/// A parser of @c char characters.
	typedef basic_parser<char> parser;

	/// A parser of @c wchar_t characters.
	typedef basic_parser<wchar_t> wparser;
} // namespace calc

#endif // CALC_PARSER_FWD_HPP
