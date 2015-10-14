/**
 * @file		parser.hpp
 * Contains type and function declarations for parsing.
 *
 * @author		Jennifer Yao
 * @date		9/25/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_PARSER_HPP
#define CALC_PARSER_HPP

#include "config.hpp"

#include <cassert>

#include "lexer.hpp"
#include "parse_error.hpp"
#include "ast.hpp"

namespace calc {
	/**
	 * Converts a sequence of tokens into an abstract syntax tree.
	 * @tparam CharT	The character type.
	 * @tparam Traits	The symbol traits type.
	 */
	template <typename CharT, class Traits>
	class basic_parser {
	public:
		typedef CharT char_type;
		typedef Traits traits_type;
		typedef typename Traits::char_traits_type char_traits_type;
		typedef typename Traits::string_type string_type;
		typedef typename Traits::string_view_type string_view_type;
		typedef typename Traits::streambuf_type streambuf_type;
		typedef typename Traits::istream_type istream_type;
		typedef basic_script_position<CharT, Traits> position_type;
		typedef basic_script_extent<CharT, Traits> extent_type;
		typedef basic_token<CharT, Traits> token_type;
		typedef basic_lexer<CharT, Traits> lexer_type;
		typedef basic_parse_error<CharT, Traits> error_type;

		/**
		 * Constructs a parser that reads from a stream buffer.
		 * @param sb	Pointer to a stream buffer.
		 */
		explicit basic_parser(streambuf_type* sb) :
			_lexer(sb), _tokens(_lexer.lex()), _next(_tokens.begin()),
			_errors()
		{}

		/**
		 * Constructs a parser that reads from an input stream.
		 * @param in	An input stream.
		 */
		explicit basic_parser(const istream_type& in) :
			_lexer(in), _tokens(_lexer.lex()), _next(_tokens.begin()),
			_errors()
		{}

		/**
		 * Parses an expression and returns a newly allocated abstract syntax
		 * tree.
		 * @return	An abstract syntax tree that represents the parsed
		 * 			expression.
		 */
		const expr* next_expr();

	private:
		lexer_type _lexer;
		std::list<token_type> _tokens;
		typename std::list<token_type>::iterator _next;
		std::list<error_type> _errors;

		lexer_type& lexer() noexcept {
			return this->_lexer;
		}

		const lexer_type& lexer() const noexcept {
			return this->_lexer;
		}

		std::list<token_type>& tokens() noexcept {
			return this->_tokens;
		}

		const std::list<token_type>& tokens() const noexcept {
			return this->_tokens;
		}

		std::list<error_type>& errors() noexcept {
			return this->_errors;
		}

		const std::list<error_type>& errors() const noexcept {
			return this->_errors;
		}

		bool eof() const {
			return this->_next == --(this->_tokens.end());
		}

		token_type& get() {
			assert(!this->eof());
			return *(this->_next++);
		}

		token_type& peek() const {
			return *(this->_next);
		}

		void unget() {
			assert(this->_next != this->_tokens.begin());
			this->_next--;
		}

		void ignore() {
			assert(!this->eof());
			this->_next++;
		}

		void skip_newlines();

		const expr* parse_expr();
		const expr* parse_factor();
		const expr* parse_term();

		void report_error(const error_type& error);
		void report_error(error_id code, const extent_type& extent, const char* message);
		void report_error(error_id code, const extent_type& extent, const std::string& message);
		void report_unexpected_token(token_type& token);
		void report_unknown_token(token_type& token);
	};
} // namespace calc

#include "parser.ipp"

#endif // CALC_PARSER_HPP
