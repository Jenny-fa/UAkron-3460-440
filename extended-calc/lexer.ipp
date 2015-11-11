/**
 * @file		lexer.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_LEXER_IPP
#define CALC_LEXER_IPP

#include <algorithm>
#include <functional>

namespace calc {
	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::next_token() {
		this->skip_blanks();

		// set token offset to current offset
		this->token_start_offset(this->offset());

		if (!this->eof()) {
			const CharT carriage_return = this->traits().widen('\r');
			const CharT line_feed = this->traits().widen('\n');

			const CharT c = Traits::to_char_type(this->peek());

			if (Traits::eq(c, carriage_return) || Traits::eq(c, line_feed))
				return this->lex_newline();
			if (this->traits().is_digit(c))
				return this->lex_integer();
			if (this->scan(this->traits().true_name()))
				return token_type(this->extent(), token_kind::boolean);
			if (this->scan(this->traits().false_name()))
				return token_type(this->extent(), token_kind::boolean);

			for (const auto& entry : this->traits().operator_table())
				if (this->scan(entry.second))
					return token_type(this->extent(), entry.first);

			return this->lex_unknown();
		}

		this->_in.setstate(std::ios_base::eofbit);
		return token_type(this->extent(), token_kind::eof);
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
	bool basic_lexer<CharT, Traits>::matches(const CharT* str, std::size_t n) {
		const std::size_t start_offset = this->offset();
		const bool result = this->scan(str, n);
		const std::size_t end_offset = this->offset();

		// set stream position to beginning of search string, if found
		if (result)
			for (std::size_t i = 0; i < end_offset - start_offset; i++)
				this->putback(this->script().back());

		return result;
	}

	template <typename CharT, class Traits>
	bool basic_lexer<CharT, Traits>::scan(const CharT* str, std::size_t n) {
		const std::size_t len = n == Traits::npos ? Traits::length(str) : n;

		bool result = true;
		std::size_t count = 0;

		while (count < len) {
			if (this->eof()
			    || !Traits::eq(str[count++], Traits::to_char_type(this->get())))
			{
				result = false;
				break;
			}
		}

		// undo extraction operation(s) if entire search string couldn't be
		// matched
		if (!result)
			for (std::size_t i = 0; i < count; i++)
				this->putback(this->script().back());

		return result;
	}

	template <typename CharT, class Traits>
	template <typename UnaryPredicate>
	bool basic_lexer<CharT, Traits>::scan_if(UnaryPredicate fn) {
		const std::size_t start_offset = this->offset();

		while (!this->eof()) {
			const CharT c = Traits::to_char_type(this->peek());
			if (!fn(c))
				break;
			this->ignore();
		}

		return start_offset < this->offset();
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_unknown() {
		assert(!this->eof());

		const CharT carriage_return = this->traits().widen('\r');
		const CharT line_feed = this->traits().widen('\n');
		const auto& operator_table = this->traits().operator_table();

		CharT c = Traits::to_char_type(this->peek());

		assert(!this->traits().is_blank(c));
		assert(!Traits::eq(c, carriage_return) && !Traits::eq(c, line_feed));
		assert(!this->traits().is_digit(c));
		assert(!this->matches(this->traits().true_name()));
		assert(!this->matches(this->traits().false_name()));

		do {
			const CharT c = Traits::to_char_type(this->peek());
			if (this->traits().is_blank(c)
			    || (Traits::eq(c, carriage_return) || Traits::eq(c, line_feed))
			    || this->traits().is_digit(c)
			    || this->matches(this->traits().true_name())
			    || this->matches(this->traits().false_name()))
				break;

			const bool matches_operator = std::any_of(operator_table.cbegin(),
				                                      operator_table.cend(),
			[this] (const auto& entry) mutable -> bool {
				return this->matches(entry.second);
			});

			if (matches_operator)
				break;

			this->ignore();
		}
		while (!this->eof());

		return token_type(this->extent(), token_kind::unknown);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_newline() {
		assert(!this->eof());

		const CharT carriage_return = this->traits().widen('\r');
		const CharT line_feed = this->traits().widen('\n');

		CharT c = Traits::to_char_type(this->get());

		assert(Traits::eq(c, carriage_return) || Traits::eq(c, line_feed));

		if (Traits::eq(c, line_feed)) {
			this->position_helper().add_line_start(this->offset());
		}
		else if (Traits::eq(c, carriage_return)) {
			if (!this->eof()) {
				c = Traits::to_char_type(this->get());
				if (Traits::eq(c, line_feed))
					this->position_helper().add_line_start(this->offset());
				else
					this->unget();
			}
		}

		return token_type(this->extent(), token_kind::newline);
	}

	template <typename CharT, class Traits>
	typename basic_lexer<CharT, Traits>::token_type
	basic_lexer<CharT, Traits>::lex_integer() {
		auto is_digit = std::bind(&Traits::is_digit, &this->traits(), std::placeholders::_1);
		assert(this->scan_if(is_digit));
		return token_type(this->extent(), token_kind::integer);
	}

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_lexer<char>;
	extern template class basic_lexer<wchar_t>;
} // namespace calc

#endif // CALC_LEXER_IPP
