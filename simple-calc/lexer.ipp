/**
 * @file		lexer.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		9/25/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_LEXER_IPP
#define CALC_LEXER_IPP

#include <algorithm>
#include <iterator>

namespace calc {
	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::next_token() {
		this->skip_blanks();

		// set token offset to current offset
		this->token_offset(this->offset());

		if (this->eof())
			return this->lex_eof();

		const CharT lf = this->_in.widen('\n');
		const CharT cr = this->_in.widen('\r');

		const CharT c = char_traits_type::to_char_type(this->peek());

		if (char_traits_type::eq(c, lf) || char_traits_type::eq(c, cr))
			return this->lex_newline();
#if !MOAR_DIGITS
		if (this->traits().is_digit(c))
			return this->lex_digit();
#else
		if (this->traits().is_digit(c))
			return this->lex_integer();
#endif
		if (this->traits().is_addition_operator(c))
			return this->lex_addition_operator();
		if (this->traits().is_subtraction_operator(c))
			return this->lex_subtraction_operator();
		if (this->traits().is_multiplication_operator(c))
			return this->lex_multiplication_operator();
		if (this->traits().is_division_operator(c))
			return this->lex_division_operator();
		if (this->traits().is_modulus_operator(c))
			return this->lex_modulus_operator();
		if (this->traits().is_left_parenthesis(c))
			return this->lex_left_parenthesis();
		if (this->traits().is_right_parenthesis(c))
			return this->lex_right_parenthesis();

		return this->lex_unknown();
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::int_type
	basic_lexer<CharT, Traits>::get() {
		// assert(this->_in.good());
		const typename char_traits_type::int_type c = this->_in.get();
		if (char_traits_type::not_eof(c))
			this->script().push_back(char_traits_type::to_char_type(c));
		if (!char_traits_type::not_eof(this->_in.peek()))
			this->_in.setstate(std::ios_base::eofbit);
		return c;
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::int_type
	basic_lexer<CharT, Traits>::peek() {
		// assert(!this->_in.fail());
		return this->_in.peek();
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::unget() {
		// assert(!this->_in.fail());
		this->_in.unget();
		if (!this->_in.bad())
			this->script().pop_back();
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::ignore() {
		// assert(this->_in.good());
		const typename char_traits_type::int_type c = this->_in.peek();
		if (char_traits_type::not_eof(c))
			this->script().push_back(char_traits_type::to_char_type(c));
		this->_in.ignore();
		if (!char_traits_type::not_eof(this->_in.peek()))
			this->_in.setstate(std::ios_base::eofbit);
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::putback(CharT c) {
		// assert(!this->_in.fail());
		this->_in.putback(c);
		if (!this->_in.bad())
			this->script().pop_back();
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::skip_blanks() {
		while (!this->eof()) {
			const CharT c = char_traits_type::to_char_type(this->peek());
			if (!this->traits().is_blank(c))
				break;
			this->ignore();
		}
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_unknown() {
		assert(!this->eof());

		const CharT lf = this->_in.widen('\n');
		const CharT cr = this->_in.widen('\r');

		do {
			const CharT c = char_traits_type::to_char_type(this->peek());
			if (this->traits().is_blank(c) || char_traits_type::eq(c, lf) || char_traits_type::eq(c, cr))
				break;
			this->ignore();
		} while (!this->eof());

		return token_type(this->extent(), token_type::kind::unknown);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_eof() {
		assert(this->eof());
		return token_type(this->extent(), token_type::kind::eof);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_newline() {
		assert(!this->eof());

		const CharT lf = this->_in.widen('\n');
		const CharT cr = this->_in.widen('\r');

		CharT c = char_traits_type::to_char_type(this->get());

		if (char_traits_type::eq(c, lf)) {
			this->_position_helper.add_line_start(this->offset());
		}
		else if (char_traits_type::eq(c, cr)) {
			if (!this->eof()) {
				c = char_traits_type::to_char_type(this->get());
				if (char_traits_type::eq(c, lf))
					this->_position_helper.add_line_start(this->offset());
				else
					this->unget();
			}
		}

		return token_type(this->extent(), token_type::kind::newline);
	}

#if !MOAR_DIGITS
	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_digit() {
		assert(!this->eof() && this->traits().is_digit(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::digit);
	}
#else
	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_integer() {
		assert(!this->eof() && this->traits().is_digit(char_traits_type::to_char_type(this->peek())));
		CharT c;
		do {
			c = char_traits_type::to_char_type(this->get());
		} while (!this->eof() && this->traits().is_digit(c));
		return token_type(this->extent(), token_type::kind::integer);
	}
#endif

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_addition_operator() {
		assert(!this->eof() && this->traits().is_addition_operator(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::addition_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_subtraction_operator() {
		assert(!this->eof() && this->traits().is_subtraction_operator(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::subtraction_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_multiplication_operator() {
		assert(!this->eof() && this->traits().is_multiplication_operator(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::multiplication_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_division_operator() {
		assert(!this->eof() && this->traits().is_division_operator(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::division_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_modulus_operator() {
		assert(!this->eof() && this->traits().is_modulus_operator(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::modulus_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_left_parenthesis() {
		assert(!this->eof() && this->traits().is_left_parenthesis(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::left_parenthesis);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_right_parenthesis() {
		assert(!this->eof() && this->traits().is_right_parenthesis(char_traits_type::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_type::kind::right_parenthesis);
	}

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_lexer<char>;
	extern template class basic_lexer<wchar_t>;
} // namespace calc

#endif // CALC_LEXER_IPP
