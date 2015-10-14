/**
 * @file		lexer.hpp
 * Contains type and function declarations for lexing.
 *
 * @author		Jennifer Yao
 * @date		9/25/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_LEXER_HPP
#define CALC_LEXER_HPP

#include "config.hpp"

#include <cassert>
#include <istream>
#include <list>

#include "token.hpp"

namespace calc {
	/**
	 * Converts a sequence of characters from an input stream into a sequence
	 * of tokens.
	 * @tparam CharT	The character type.
	 * @tparam Traits	The symbol traits type.
	 */
	template <typename CharT, class Traits>
	class basic_lexer {
		friend class basic_parser<CharT, Traits>;

	public:
		typedef CharT char_type;
		typedef Traits traits_type;
		typedef typename Traits::char_traits_type char_traits_type;
		typedef typename char_traits_type::int_type int_type;
		typedef typename Traits::string_type string_type;
		typedef typename Traits::string_view_type string_view_type;
		typedef typename Traits::streambuf_type streambuf_type;
		typedef typename Traits::istream_type istream_type;
		typedef basic_script_position<CharT, Traits> position_type;
		typedef basic_script_extent<CharT, Traits> extent_type;
		typedef basic_token<CharT, Traits> token_type;

	public:
		/**
		 * Constructs a lexer that will read from a stream buffer.
		 * @param sb	Pointer to a stream buffer.
		 */
		explicit basic_lexer(streambuf_type* sb) :
			_traits(), _in(sb), _position_helper(), _token_offset(0)
		{
			this->_in.exceptions(std::ios_base::failbit);
		}

		/**
		 * Constructs a lexer that will read from an input stream.
		 * @param in	An input stream.
		 */
		explicit basic_lexer(const istream_type& in) :
			_traits(), _in(in.rdbuf()), _position_helper(), _token_offset(0)
		{
			// copy other input stream's state except for exception mask and
			// unset failbit
			this->_in.copyfmt(in);
			this->_in.clear(in.rdstate() & ~std::ios_base::failbit);
			this->_in.exceptions(std::ios_base::failbit);
		}

		/**
		 * Extracts tokens from the input stream until the end-of-file is
		 * reached, and returns the extracted tokens as a list.
		 *
		 * The kind of the last token will always be token_base::kind::eof.
		 *
		 * @return	A list of extracted tokens.
		 */
		std::list<token_type> lex() {
			std::list<token_type> tokens;
			while (true) {
				const token_type token = this->next_token();
				tokens.push_back(token);
				if (token.kind() == token_base::kind::eof)
					break;
			}
			assert(!tokens.empty() && tokens.back().kind() == token_base::kind::eof);
			return tokens;
		}

		/**
		 * Extracts tokens from the input stream until the end of the line is
		 * reached, and returns the extracted tokens as a list.
		 *
		 * The newline token is extracted but not stored in the returned list.
		 *
		 * @return	A list of extracted tokens.
		 */
		std::list<token_type> lex_line() {
			std::list<token_type> tokens;
			while (true) {
				const token_type token = this->next_token();
				if (token.kind() == token_base::kind::newline)
					break;
				tokens.push_back(token);
			}
			return tokens;
		}

		/**
		 * Extracts the next token from the input stream.
		 * @return	The extracted token.
		 */
		token_type next_token();

	private:
		typedef basic_script_position_helper<CharT, Traits> position_helper_type;

		/// The symbol traits object.
		Traits _traits;
		/// The input stream.
		istream_type _in;
		/// A helper object.
		position_helper_type _position_helper;
		/// The currently scanned token's offset from the beginning of the
		/// script.
		std::size_t _token_offset;

		Traits& traits() noexcept {
			return this->_traits;
		}

		const Traits& traits() const noexcept {
			return this->_traits;
		}

		position_helper_type& position_helper() noexcept {
			return this->_position_helper;
		}

		const position_helper_type& position_helper() const noexcept {
			return this->_position_helper;
		}

		string_type& script() noexcept {
			return this->position_helper().script();
		}

		const string_type& script() const noexcept {
			return this->position_helper().script();
		}

		std::size_t offset() const noexcept {
			return this->script().size();
		}

		std::size_t token_offset() const noexcept {
			return this->_token_offset;
		}

		void token_offset(std::size_t offset) noexcept {
			assert(offset <= this->offset());
			this->_token_offset = offset;
		}

		extent_type extent() const noexcept {
			return extent_type(this->position_helper(), this->token_offset(), this->offset());
		}

		bool eof() const {
			return this->_in.eof();
		}

		int_type get();
		int_type peek();
		void unget();
		void ignore();
		void putback(CharT c);

		void skip_blanks();

		token_type lex_unknown();
		token_type lex_eof();
		token_type lex_newline();
#if !MOAR_DIGITS
		token_type lex_digit();
#else
		token_type lex_integer();
#endif
		token_type lex_addition_operator();
		token_type lex_subtraction_operator();
		token_type lex_multiplication_operator();
		token_type lex_division_operator();
		token_type lex_modulus_operator();
		token_type lex_left_parenthesis();
		token_type lex_right_parenthesis();
	};
} // namespace calc

#include "lexer.ipp"

#endif // CALC_LEXER_HPP
