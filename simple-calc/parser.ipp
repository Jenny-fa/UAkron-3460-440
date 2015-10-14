/**
 * @file		parser.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/10/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_PARSER_IPP
#define CALC_PARSER_IPP

#include <algorithm>

#include "numeric_conversions.hpp"

namespace calc {
	template <typename CharT, class Traits>
	const expr* basic_parser<CharT, Traits>::next_expr() {
		this->skip_newlines();
		if (this->eof())
			return nullptr;
		return this->parse_expr();
	}

	template <typename CharT, class Traits>
	void basic_parser<CharT, Traits>::skip_newlines() {
		while (!this->eof()) {
			token_type& token = this->peek();
			if (token.kind() != token_base::kind::newline)
				break;
			this->ignore();
		}
	}

	template <typename CharT, class Traits>
	const expr* basic_parser<CharT, Traits>::parse_expr() {
		assert(!this->eof());

		const expr* result = this->parse_factor();

		while (!this->eof()) {
			const expr* rest = nullptr;
			token_type& token = this->get();

			switch (token.kind()) {
				case token_base::kind::addition_operator:
					rest = this->parse_factor();
					result = new addition_expr(result, rest);
					break;
				case token_base::kind::subtraction_operator:
					rest = this->parse_factor();
					result = new subtraction_expr(result, rest);
					break;
				default:
					this->unget();
					goto exit1;
			}
		}
exit1:
		return result;
	}

	template <typename CharT, class Traits>
	const expr* basic_parser<CharT, Traits>::parse_factor() {
		assert(!this->eof());

		const expr* result = this->parse_term();

		while (!this->eof()) {
			const expr* rest = nullptr;
			token_type& token = this->get();

			switch (token.kind()) {
				case token_base::kind::multiplication_operator:
					rest = this->parse_term();
					result = new multiplication_expr(result, rest);
					break;
				case token_base::kind::division_operator:
					rest = this->parse_term();
					result = new division_expr(result, rest);
					break;
				case token_base::kind::modulus_operator:
					rest = this->parse_term();
					result = new modulus_expr(result, rest);
					break;
				default:
					this->unget();
					goto exit2;
			}
		}
exit2:
		return result;
	}

	template <typename CharT, class Traits>
	const expr* basic_parser<CharT, Traits>::parse_term() {
		assert(!this->eof());

		const expr* result = nullptr;
		token_type& token = this->get();

		switch (token.kind()) {
#if !MOAR_DIGITS
			case token_base::kind::digit:
				result = new digit(stoi(token.text().to_string()));
				break;
#else
			case token_base::kind::integer:
				result = new integer(stoll(token.text().to_string()));
				break;
#endif
			case token_base::kind::left_parenthesis:
				result = this->parse_expr();
				if (this->peek().kind() == token_base::kind::right_parenthesis)
					this->ignore();
				else {
					token.flags(token_base::flags::has_error);
					this->report_error(error_id::missing_closing_parenthesis,
						extent_type(this->lexer().position_helper(), token.extent().start_offset(), this->peek().extent().end_offset()),
						"Nested expression is missing closing parenthesis.");
				}
				break;
			case token_base::kind::unknown:
				this->report_unknown_token(token);
				break;
			default:
				this->report_unexpected_token(token);
				break;
		}

		return result;
	}

	template <typename CharT, class Traits>
	void
	basic_parser<CharT, Traits>::report_error(const typename basic_parser<CharT, Traits>::error_type& error) {
		auto is_equal = [&error] (const error_type& i) {
			return i.code() == error.code() && i.extent() == error.extent();
		};
		if (std::none_of(this->errors().cbegin(), this->errors().cend(), is_equal))
			this->errors().push_back(error);
		// TODO: Fix potential memory leaks!
		throw error;
	}

	template <typename CharT, class Traits>
	void
	basic_parser<CharT, Traits>::report_error(error_id code,
	  const typename basic_parser<CharT, Traits>::extent_type& extent,
	  const char* message)
	{
		this->report_error(error_type(code, extent, message));
	}

	template <typename CharT, class Traits>
	void
	basic_parser<CharT, Traits>::report_error(error_id code,
	  const typename basic_parser<CharT, Traits>::extent_type& extent,
	  const std::string& message)
	{
		this->report_error(error_type(code, extent, message));
	}

	template <typename CharT, class Traits>
	void
	basic_parser<CharT, Traits>::report_unexpected_token(typename basic_parser<CharT, Traits>::token_type& token) {
		token.flags(token_base::flags::has_error);
		this->report_error(error_id::unexpected_token, token.extent(), "Unexpected token.");
	}

	template <typename CharT, class Traits>
	void
	basic_parser<CharT, Traits>::report_unknown_token(typename basic_parser<CharT, Traits>::token_type& token) {
		token.flags(token_base::flags::has_error);
		this->report_error(error_id::unknown_token, token.extent(), "Unrecognized token.");
	}

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_parser<char>;
	extern template class basic_parser<wchar_t>;
} // namespace calc

#endif // CALC_PARSER_IPP
