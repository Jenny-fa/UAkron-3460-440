/**
 * @file		symbol_traits.hpp
 * I don't even know anymore.
 *
 * @author		Jennifer Yao
 * @date		10/12/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_SYMBOL_TRAITS_HPP
#define CALC_SYMBOL_TRAITS_HPP

#include "config.hpp"

#include <iosfwd>
#include <string>
#include <experimental/string_view>
#include <locale>

namespace calc {
	template <typename CharT> class symbol_traits;
	template <> class symbol_traits<char>;
	template <> class symbol_traits<wchar_t>;

	/**
	 * The type trait template symbol_traits supplies basic_lexer and
	 * basic_parser with the set of types and functions necessary to operate
	 * on the character type @c CharT.
	 * @tparam CharT	The character type.
	 */
	template <typename CharT>
	class symbol_traits {
	public:
		typedef CharT char_type;
		typedef std::char_traits<CharT> char_traits_type;
		typedef typename std::char_traits<CharT>::int_type int_type;
		typedef std::basic_string<CharT> string_type;
		typedef std::experimental::basic_string_view<CharT> string_view_type;
		typedef std::basic_streambuf<CharT> streambuf_type;
		typedef std::basic_istream<CharT> istream_type;
		typedef std::basic_ostream<CharT> ostream_type;
		typedef std::locale locale_type;

		symbol_traits() : _locale() {}

		locale_type getloc() const {
			return this->_locale;
		}

		locale_type imbue(const locale_type& loc) {
			this->_locale = loc;
		}

		bool is_blank(CharT c) const;
		bool is_newline_start(CharT c) const;
		bool is_digit(CharT c) const;
		bool is_addition_operator(CharT c) const;
		bool is_subtraction_operator(CharT c) const;
		bool is_multiplication_operator(CharT c) const;
		bool is_division_operator(CharT c) const;
		bool is_modulus_operator(CharT c) const;
		bool is_left_parenthesis(CharT c) const;
		bool is_right_parenthesis(CharT c) const;

	private:
		locale_type _locale;
	};

	template <>
	class symbol_traits<char> {
	public:
		typedef char char_type;
		typedef std::char_traits<char> char_traits_type;
		typedef typename std::char_traits<char>::int_type int_type;
		typedef std::basic_string<char> string_type;
		typedef std::experimental::basic_string_view<char> string_view_type;
		typedef std::basic_streambuf<char> streambuf_type;
		typedef std::basic_istream<char> istream_type;
		typedef std::basic_ostream<char> ostream_type;
		typedef std::locale locale_type;

		symbol_traits() : _locale() {}

		locale_type getloc() const {
			return this->_locale;
		}

		locale_type imbue(const locale_type& loc) {
			this->_locale = loc;
		}

		bool is_blank(char c) const {
#if HAVE_STD_ISBLANK
		return std::isblank<char>(c, this->_locale);
#else
		return char_traits_type::eq(c, ' ') || char_traits_type::eq(c, '\t');
#endif
		}

		bool is_newline_start(char c) const {
			return char_traits_type::eq(c, '\n') || char_traits_type::eq(c, '\r');
		}

		bool is_digit(char c) const {
			return std::isdigit(c, this->_locale);
		}

		bool is_addition_operator(char c) const {
			return char_traits_type::eq(c, '+');
		}

		bool is_subtraction_operator(char c) const {
			return char_traits_type::eq(c, '-');
		}

		bool is_multiplication_operator(char c) const {
			return char_traits_type::eq(c, '*');
		}

		bool is_division_operator(char c) const {
			return char_traits_type::eq(c, '/');
		}

		bool is_modulus_operator(char c) const {
			return char_traits_type::eq(c, '%');
		}

		bool is_left_parenthesis(char c) const {
			return char_traits_type::eq(c, '(');
		}

		bool is_right_parenthesis(char c) const {
			return char_traits_type::eq(c, ')');
		}

	private:
		locale_type _locale;
	};

	template <>
	class symbol_traits<wchar_t> {
	public:
		typedef wchar_t char_type;
		typedef std::char_traits<wchar_t> char_traits_type;
		typedef typename std::char_traits<wchar_t>::int_type int_type;
		typedef std::basic_string<wchar_t> string_type;
		typedef std::experimental::basic_string_view<wchar_t> string_view_type;
		typedef std::basic_streambuf<wchar_t> streambuf_type;
		typedef std::basic_istream<wchar_t> istream_type;
		typedef std::basic_ostream<wchar_t> ostream_type;
		typedef std::locale locale_type;

		symbol_traits() : _locale() {}

		locale_type getloc() const {
			return this->_locale;
		}

		locale_type imbue(const locale_type& loc) {
			this->_locale = loc;
		}

		bool is_blank(wchar_t c) const {
#if HAVE_STD_ISBLANK
		return std::isblank<wchar_t>(c, this->_locale);
#else
		return char_traits_type::eq(c, L' ') || char_traits_type::eq(c, '\t');
#endif
		}

		bool is_newline_start(wchar_t c) const {
			return char_traits_type::eq(c, L'\n') || char_traits_type::eq(c, L'\r');
		}

		bool is_digit(wchar_t c) const {
			return std::isdigit(c, this->_locale);
		}

		bool is_addition_operator(wchar_t c) const {
			return char_traits_type::eq(c, L'+');
		}

		bool is_subtraction_operator(wchar_t c) const {
			return char_traits_type::eq(c, L'-');
		}

		bool is_multiplication_operator(wchar_t c) const {
			return char_traits_type::eq(c, L'*');
		}

		bool is_division_operator(wchar_t c) const {
			return char_traits_type::eq(c, L'/');
		}

		bool is_modulus_operator(wchar_t c) const {
			return char_traits_type::eq(c, L'%');
		}

		bool is_left_parenthesis(wchar_t c) const {
			return char_traits_type::eq(c, L'(');
		}

		bool is_right_parenthesis(wchar_t c) const {
			return char_traits_type::eq(c, L')');
		}

	private:
		locale_type _locale;
	};
} // namespace calc

#include "symbol_traits.ipp"

#endif // CALC_SYMBOL_TRAITS_HPP
