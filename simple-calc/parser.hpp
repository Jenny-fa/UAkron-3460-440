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
#include <memory>

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
		typedef typename Traits::string_type string_type;
		typedef typename Traits::string_view_type string_view_type;
		typedef typename Traits::streambuf_type streambuf_type;
		typedef typename Traits::istream_type istream_type;
		typedef typename Traits::locale_type locale_type;
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
			_lexer(sb), _expr_start_offset(0), _tokens(), _errors()
		{}

		/**
		 * Constructs a parser that reads from an input stream.
		 * @param in	An input stream.
		 */
		explicit basic_parser(const istream_type& in) :
			basic_parser(in.rdbuf())
		{}

		/**
		 * Parses an expression and returns a newly allocated abstract syntax
		 * tree.
		 * @param skip_newlines	@c true if empty lines preceding the
		 * 						expression should be skipped. Defaults to
		 * 						@c false.
		 * @return				An abstract syntax tree that represents the
		 * 						parsed expression.
		 */
		std::unique_ptr<const expr> next_expr(bool skip_newlines = false);

		/**
		 * Returns the current locale associated with the parser.
		 * @return	The current locale associated with the parser.
		 */
		locale_type getloc() const {
			return this->lexer().getloc();
		}

		/**
		 * Replaces the current locale with a copy of @p loc.
		 * @param loc	The locale that will replace the current locale.
		 * @return		The locale before the call to this function.
		 */
		locale_type imbue(const locale_type& loc) {
			return this->lexer().imbue(loc);
		}

		/**
		 * Returns true if the associated input stream has no errors and the
		 * parser is ready for parsing.
		 * @return	@c true if the input stream has no errors, @c false
		 * 			otherwise.
		 */
		explicit operator bool() const {
			return static_cast<bool>(this->lexer());
		}

	private:
		typedef basic_script_position_helper<CharT, Traits> position_helper_type;

		lexer_type _lexer;
		std::size_t _expr_start_offset;
		std::list<token_type> _tokens;
		std::list<error_type> _errors;

		lexer_type& lexer() noexcept {
			return this->_lexer;
		}

		const lexer_type& lexer() const noexcept {
			return this->_lexer;
		}

		const position_helper_type& position_helper() const noexcept {
			return this->lexer().position_helper();
		}

		const string_type& script() const noexcept {
			return this->position_helper().script();
		}

		std::size_t offset() const noexcept {
			return this->peek().extent().start_offset();
		}

		std::size_t expr_start_offset() const noexcept {
			return this->_expr_start_offset;
		}

		void expr_start_offset(std::size_t offset) noexcept {
			assert(offset <= this->offset());
			this->_expr_start_offset = offset;
		}

		extent_type extent() const noexcept {
			return extent_type(this->position_helper(), this->expr_start_offset(), this->offset());
		}

		extent_type extent_from(std::size_t start_offset) const noexcept {
			return extent_type(this->position_helper(), start_offset, this->offset());
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
			return this->peek().kind() == token_base::kind::eof;
		}

		token_type& get() {
			assert(!this->eof());
			token_type& next_token = this->peek();
			this->tokens().push_back(this->lexer().next_token());
			return next_token;
		}

		token_type& peek() {
			assert(!this->tokens().empty());
			return this->tokens().back();
		}

		const token_type& peek() const {
			assert(!this->tokens().empty());
			return this->tokens().back();
		}

		void unget() {
			string_view_type next_token_text = this->peek().text();
			for (auto itr = next_token_text.rbegin(); itr != next_token_text.rend(); ++itr)
				this->lexer().putback(*itr);
			this->tokens().pop_back();
			this->lexer().rewind_blanks();
		}

		void ignore() {
			assert(!this->eof());
			this->tokens().push_back(this->lexer().next_token());
		}

		std::unique_ptr<const expr> parse_expr();
		std::unique_ptr<const expr> parse_factor();
		std::unique_ptr<const expr> parse_term();

		void report_error(const error_type& error);
		void report_error(error_id code, const extent_type& extent, const char* message);
		void report_error(error_id code, const extent_type& extent, const std::string& message);
	};
} // namespace calc

#include "parser.ipp"

#endif // CALC_PARSER_HPP
