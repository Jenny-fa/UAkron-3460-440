/**
 * @file		symbol_traits.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/12/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_SYMBOL_TRAITS_IPP
#define CALC_SYMBOL_TRAITS_IPP

namespace calc {
	template <typename CharT>
	inline bool symbol_traits<CharT>::is_blank(char_type c) const {
#if HAVE_STD_ISBLANK
		return std::isblank<CharT>(c, this->_locale);
#else
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		const CharT space = ctype_facet.widen(' ');
		const CharT tab = ctype_facet.widen('\t');
		return char_traits_type::eq(c, space) || char_traits_type::eq(c, tab);
#endif
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_newline_start(char_type c) const {
		return this->is_line_feed(c) || this->is_carriage_return(c);
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_line_feed(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('\n'));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_carriage_return(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('\r'));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_digit(char_type c) const {
		return std::isdigit(c, this->_locale);
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_addition_operator(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('+'));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_subtraction_operator(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('-'));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_multiplication_operator(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('*'));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_division_operator(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('/'));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_modulus_operator(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('%'));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_left_parenthesis(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen('('));
	}

	template <typename CharT>
	inline bool symbol_traits<CharT>::is_right_parenthesis(char_type c) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		return char_traits_type::eq(c, ctype_facet.widen(')'));
	}
} // namespace calc

#endif // CALC_SYMBOL_TRAITS_IPP
