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
		this->token_start_offset(this->offset());

		if (!this->eof()) {
			const CharT c = Traits::to_char_type(this->peek());

			if (this->traits().is_newline_start(c))
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

		this->_in.setstate(std::ios_base::eofbit);
		return this->lex_eof();
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::int_type
	basic_lexer<CharT, Traits>::get() {
		const int_type c = this->_in.get();
		if (!Traits::is_eof(c))
			this->script().push_back(Traits::to_char_type(c));
		return c;
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::int_type
	basic_lexer<CharT, Traits>::peek() {
		return this->_in.peek();
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::unget() {
		this->_in.unget();
		if (!this->_in.bad())
			this->script().pop_back();
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::ignore() {
		const int_type c = this->_in.peek();
		if (!Traits::is_eof(c))
			this->script().push_back(Traits::to_char_type(c));
		this->_in.ignore();
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::putback(CharT c) {
		this->_in.putback(c);
		if (!this->_in.bad())
			this->script().pop_back();
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::skip_blanks() {
		while (!this->eof()) {
			const CharT c = Traits::to_char_type(this->peek());
			if (!this->traits().is_blank(c))
				break;
			this->ignore();
		}
	}

	template <typename CharT, class Traits>
	void basic_lexer<CharT, Traits>::rewind_blanks() {
		while (!this->script().empty()) {
			const CharT c = this->script().back();
			if (!this->traits().is_blank(c))
				break;
			this->putback(c);
		}
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_unknown() {
		assert(!this->eof());

		do {
			const CharT c = Traits::to_char_type(this->peek());
			if (this->traits().is_blank(c) || this->traits().is_newline_start(c))
				break;
			this->ignore();
		} while (!this->eof());

		return token_type(this->extent(), token_kind::unknown);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_eof() {
		assert(this->eof());
		return token_type(this->extent(), token_kind::eof);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_newline() {
		assert(!this->eof() && this->traits().is_newline_start(Traits::to_char_type(this->peek())));

		CharT c = Traits::to_char_type(this->get());

		if (this->traits().is_line_feed(c)) {
			this->position_helper().add_line_start(this->offset());
		}
		else if (this->traits().is_carriage_return(c)) {
			if (!this->eof()) {
				c = Traits::to_char_type(this->get());
				if (this->traits().is_line_feed(c))
					this->position_helper().add_line_start(this->offset());
				else
					this->unget();
			}
		}

		return token_type(this->extent(), token_kind::newline);
	}

#if !MOAR_DIGITS
	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_digit() {
		assert(!this->eof() && this->traits().is_digit(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::digit);
	}
#else
	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_integer() {
		assert(!this->eof() && this->traits().is_digit(Traits::to_char_type(this->peek())));
		do {
			const CharT c = Traits::to_char_type(this->peek());
			if (!this->traits().is_digit(c))
				break;
			this->ignore();
		} while (!this->eof());
		return token_type(this->extent(), token_kind::integer);
	}
#endif

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_addition_operator() {
		assert(!this->eof() && this->traits().is_addition_operator(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::addition_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_subtraction_operator() {
		assert(!this->eof() && this->traits().is_subtraction_operator(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::subtraction_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_multiplication_operator() {
		assert(!this->eof() && this->traits().is_multiplication_operator(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::multiplication_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_division_operator() {
		assert(!this->eof() && this->traits().is_division_operator(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::division_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_modulus_operator() {
		assert(!this->eof() && this->traits().is_modulus_operator(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::modulus_operator);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_left_parenthesis() {
		assert(!this->eof() && this->traits().is_left_parenthesis(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::left_parenthesis);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_right_parenthesis() {
		assert(!this->eof() && this->traits().is_right_parenthesis(Traits::to_char_type(this->peek())));
		this->ignore();
		return token_type(this->extent(), token_kind::right_parenthesis);
	}

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_lexer<char>;
	extern template class basic_lexer<wchar_t>;
} // namespace calc

#endif // CALC_LEXER_IPP
