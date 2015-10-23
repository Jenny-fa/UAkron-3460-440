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
	std::unique_ptr<const expr> basic_parser<CharT, Traits>::next_expr() {
		// lazily extract first token from input stream
		if (this->tokens().empty())
			this->tokens().push_back(this->lexer().next_token());
		this->skip_newlines();
		if (this->eof())
			return std::unique_ptr<const expr>();
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
	std::unique_ptr<const expr> basic_parser<CharT, Traits>::parse_expr() {
		this->expr_start_offset(this->offset());

		std::unique_ptr<const expr> result = this->parse_factor();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_base::kind::addition_operator:
					this->ignore();
					rest = this->parse_factor();
					result = std::make_unique<addition_expr>(std::move(result), std::move(rest));
					break;
				case token_base::kind::subtraction_operator:
					this->ignore();
					rest = this->parse_factor();
					result = std::make_unique<subtraction_expr>(std::move(result), std::move(rest));
					break;
				default:
					goto exit;
			}
		}
exit:
		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr> basic_parser<CharT, Traits>::parse_factor() {
		std::unique_ptr<const expr> result = this->parse_term();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_base::kind::multiplication_operator:
					this->ignore();
					rest = this->parse_term();
					result = std::make_unique<multiplication_expr>(std::move(result), std::move(rest));
					break;
				case token_base::kind::division_operator:
					this->ignore();
					rest = this->parse_term();
					result = std::make_unique<division_expr>(std::move(result), std::move(rest));
					break;
				case token_base::kind::modulus_operator:
					this->ignore();
					rest = this->parse_term();
					result = std::make_unique<modulus_expr>(std::move(result), std::move(rest));
					break;
				default:
					goto exit;
			}
		}
exit:
		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr> basic_parser<CharT, Traits>::parse_term() {
		std::unique_ptr<const expr> result = nullptr;
		token_type& token = this->peek();

		switch (token.kind()) {
#if !MOAR_DIGITS
			case token_base::kind::digit:
				result = std::make_unique<digit>(stoi(token.text().to_string()));
				this->ignore();
				break;
#else
			case token_base::kind::integer:
				result = std::make_unique<integer>(stoll(token.text().to_string()));
				this->ignore();
				break;
#endif
			case token_base::kind::left_parenthesis:
				this->ignore();
				result = this->parse_expr();
				if (this->peek().kind() == token_base::kind::right_parenthesis)
					this->ignore();
				else {
					token.flags(token_base::flags::has_error);
					this->report_error(error_id::missing_closing_parenthesis,
						extent_type(this->position_helper(), token.extent().start_offset(), this->offset()),
						"Nested expression is missing closing parenthesis.");
				}
				break;
			case token_base::kind::unknown:
				this->ignore();
				token.flags(token_base::flags::has_error);
				this->report_error(error_id::unknown_token, token.extent(), "Unrecognized token.");
				break;
			case token_base::kind::eof:
				token.flags(token_base::flags::has_error);
				this->report_error(error_id::unexpected_token, token.extent(), "Unexpected end of file.");
				break;
			case token_base::kind::newline:
				token.flags(token_base::flags::has_error);
				this->report_error(error_id::unexpected_token, token.extent(), "Unexpected end of line.");
				break;
			default:
				this->ignore();
				token.flags(token_base::flags::has_error);
				this->report_error(error_id::unexpected_token, token.extent(), "Unexpected token.");
				break;
		}

		return std::move(result);
	}

	template <typename CharT, class Traits>
	void
	basic_parser<CharT, Traits>::report_error(const typename basic_parser<CharT, Traits>::error_type& error) {
		auto error_equal = [&error] (const error_type& i) {
			return i.code() == error.code() && i.extent() == error.extent();
		};
		if (std::none_of(this->errors().cbegin(), this->errors().cend(), error_equal))
			this->errors().push_back(error);
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

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_parser<char>;
	extern template class basic_parser<wchar_t>;
} // namespace calc

#endif // CALC_PARSER_IPP
