/**
 * @file		symbol_traits.hpp
 * I don't even know anymore.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_SYMBOL_TRAITS_HPP
#define CALC_SYMBOL_TRAITS_HPP

#include "config.hpp"

#include <cstdint>
#include <iosfwd>
#include <locale>
#include <map>
#include <string>
#include <experimental/string_view>

#include "constants.hpp"
#include "numeric_conversions.hpp"

namespace calc {
	template <typename CharT> class symbol_traits;

	/**
	 * Provides constants that are used by the symbol_traits class template.
	 */
	class symbol_base {
	public:
		static const std::string newlines[3];
		static const std::map<token_kind, std::string> operator_table;
	};

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

		static constexpr std::size_t npos = std::size_t(-1);

		static std::size_t length(const char_type* str) {
			return char_traits_type::length(str);
		}

		static constexpr bool eq(char_type c1, char_type c2) noexcept {
			return char_traits_type::eq(c1, c2);
		}

		static constexpr char_type to_char_type(int_type c) noexcept {
			return char_traits_type::to_char_type(c);
		}

		static constexpr bool is_eof(int_type c) noexcept {
			return char_traits_type::eq_int_type(c, char_traits_type::eof());
		}

		symbol_traits() {
			this->init();
		}

		locale_type getloc() const {
			return this->_locale;
		}

		locale_type imbue(const locale_type& loc) {
			locale_type temp(this->_locale);
			this->_locale = loc;
			this->init(); // reinitialize cached strings
			return temp;
		}

		char_type widen(char c) const {
			const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
			return ctype_facet.widen(c);
		}

		string_type widen(const std::string& str) const;

		bool is_blank(char_type c) const {
#if HAVE_STD_ISBLANK
			return std::isblank<CharT>(c, this->_locale);
#else
			return symbol_traits::eq(c, this->widen(' '))
				|| symbol_traits::eq(c, this->widen('\t'));
#endif
		}

		bool is_digit(char_type c) const {
			return std::isdigit(c, this->_locale);
		}

		bool bool_value(const string_type& str, std::size_t* idx = nullptr) const;

		std::int32_t int32_value(const string_type& str, std::size_t* idx = nullptr, int base = 10) const {
#if HAVE_INT32_T_INT
			return stoi(str, idx, base);
#else
			return stol(str, idx, base);
#endif
		}

		const string_type (&newlines() const noexcept)[3] {
			return this->_newlines;
		}

		const string_type& true_name() const noexcept {
			return this->_true_name;
		}

		const string_type& false_name() const noexcept {
			return this->_false_name;
		}

		const std::map<token_kind, string_type>& operator_table() const noexcept {
			return this->_operator_table;
		}

	private:
		locale_type _locale;
		string_type _newlines[3];
		string_type _true_name;
		string_type _false_name;
		std::map<token_kind, string_type> _operator_table;

		void init();
	};
} // namespace calc

#include "symbol_traits.ipp"

#endif // CALC_SYMBOL_TRAITS_HPP
