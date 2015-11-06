/**
 * @file		lexer_fwd.hpp
 * Contains forward declarations for the basic_lexer class.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_LEXER_FWD_HPP
#define CALC_LEXER_FWD_HPP

namespace calc {
	template <typename CharT> class symbol_traits;

	template <typename CharT, class Traits = symbol_traits<CharT>>
	class basic_lexer;

	/// A lexer of @c char characters.
	typedef basic_lexer<char> lexer;

	/// A lexer of @c wchar_t characters.
	typedef basic_lexer<wchar_t> wlexer;
} // namespace calc

#endif // CALC_LEXER_FWD_HPP
