/**
 * @file		parser.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_PARSER_IPP
#define CALC_PARSER_IPP

#include <algorithm>

namespace calc {
	template <typename CharT, class Traits>
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::next_expr(bool skip_newlines) {
		// lazily extract first token from input stream
		if (this->tokens().empty())
			this->tokens().push_back(this->lexer().next_token());
		// skip newline at the end of the previous expression
		else if (this->peek().kind() == token_kind::newline)
			this->ignore();

		if (skip_newlines)
			while (!this->eof() && this->peek().kind() == token_kind::newline)
				this->ignore();

		if (this->eof())
			return std::unique_ptr<const expr>();

		// check whether newline follows expression
		std::unique_ptr<const expr> result = this->parse_expr();
		token_type& token = this->peek();

		if (token.kind() != token_kind::newline) {
			// skip the rest of the tokens in this line
			while (!this->eof() && this->peek().kind() != token_kind::newline)
				this->ignore();
			token.flags(token.flags() | token_flags::has_error);
			this->report_error(error_id::unexpected_token, this->extent_from(token.extent().start_offset()), "Expected newline before expression.");
		}

		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr> basic_parser<CharT, Traits>::parse_expr() {
		return this->parse_logical_or_expr();
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_primary_expr() {
		std::unique_ptr<const expr> result = nullptr;
		token_type& token = this->peek();

		switch (token.kind()) {
			case token_kind::boolean:
				result = std::make_unique<boolean>(this->traits().bool_value(token.text().to_string()));
				this->ignore();
				break;
			case token_kind::integer:
				try {
					result = std::make_unique<integer>(this->traits().int32_value(token.text().to_string()));
					this->ignore();
				}
				catch (const std::out_of_range& exception) {
					this->ignore();
					token.flags(token.flags() | token_flags::has_error);
					this->report_error(error_id::integer_out_of_range, token.extent(), "Integer literal is outside the range of -(2^31) to 2^31 - 1.");
				}
				break;
			case token_kind::left_parenthesis:
				this->ignore();
				result = this->parse_expr();
				if (this->peek().kind() == token_kind::right_parenthesis)
					this->ignore();
				else {
					token.flags(token.flags() | token_flags::has_error);
					this->report_error(error_id::missing_end_parenthesis, this->extent_from(token.extent().start_offset()), "Expression in parentheses is missing ')'.");
				}
				break;
			case token_kind::eof:
				token.flags(token.flags() | token_flags::has_error);
				this->report_error(error_id::unexpected_token, token.extent(), "Unexpected end of file.");
				break;
			case token_kind::newline:
				token.flags(token.flags() | token_flags::has_error);
				this->report_error(error_id::unexpected_token, token.extent(), "Unexpected end of line.");
				break;
			case token_kind::unknown:
				this->ignore();
				token.flags(token.flags() | token_flags::has_error);
				this->report_error(error_id::unknown_token, token.extent(), "Unrecognized token.");
				break;
			default:
				this->ignore();
				token.flags(token.flags() | token_flags::has_error);
				this->report_error(error_id::unexpected_token, token.extent(), "Unexpected token.");
				break;
		}

		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_unary_expr() {
		std::unique_ptr<const expr> result = nullptr;
		token_type& token = this->peek();

		switch (token.kind()) {
			case token_kind::positive_or_addition_operator:
				// set token flags for unary plus operator
				token.flags((token.flags() & ~(token_flags::operator_associativity_mask | token_flags::binary_operator_mask)) | token_flags::right_associative);
				this->ignore();
				result = std::make_unique<positive_expr>(this->parse_unary_expr());
				break;
			case token_kind::negative_or_subtraction_operator:
				// set token flags for unary negation operator
				token.flags((token.flags() & ~(token_flags::operator_associativity_mask | token_flags::binary_operator_mask)) | token_flags::right_associative);
				this->ignore();
				result = std::make_unique<negative_expr>(this->parse_unary_expr());
				break;
			case token_kind::logical_not_operator:
				this->ignore();
				result = std::make_unique<logical_not_expr>(this->parse_unary_expr());
				break;
			default:
				result = this->parse_primary_expr();
				break;
		}

		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_multiplicative_expr() {
		std::unique_ptr<const expr> result = this->parse_unary_expr();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_kind::multiplication_operator:
					this->ignore();
					rest = this->parse_unary_expr();
					result = std::make_unique<multiplication_expr>(std::move(result), std::move(rest));
					break;
				case token_kind::division_operator:
					this->ignore();
					rest = this->parse_unary_expr();
					result = std::make_unique<division_expr>(std::move(result), std::move(rest));
					break;
				case token_kind::modulus_operator:
					this->ignore();
					rest = this->parse_unary_expr();
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
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_additive_expr() {
		std::unique_ptr<const expr> result = this->parse_multiplicative_expr();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_kind::positive_or_addition_operator:
					// set token flags for binary addition operator
					token.flags((token.flags() & ~(token_flags::operator_associativity_mask | token_flags::unary_operator_mask)) | token_flags::left_associative);
					this->ignore();
					rest = this->parse_multiplicative_expr();
					result = std::make_unique<addition_expr>(std::move(result), std::move(rest));
					break;
				case token_kind::negative_or_subtraction_operator:
					// set token flags for binary subtraction operator
					token.flags((token.flags() & ~(token_flags::operator_associativity_mask | token_flags::unary_operator_mask)) | token_flags::left_associative);
					this->ignore();
					rest = this->parse_multiplicative_expr();
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
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_ordering_expr() {
		std::unique_ptr<const expr> result = this->parse_additive_expr();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_kind::less_operator:
					this->ignore();
					rest = this->parse_additive_expr();
					result = std::make_unique<less_expr>(std::move(result), std::move(rest));
					break;
				case token_kind::greater_operator:
					this->ignore();
					rest = this->parse_additive_expr();
					result = std::make_unique<greater_expr>(std::move(result), std::move(rest));
					break;
				case token_kind::less_equal_operator:
					this->ignore();
					rest = this->parse_additive_expr();
					result = std::make_unique<less_equal_expr>(std::move(result), std::move(rest));
					break;
				case token_kind::greater_equal_operator:
					this->ignore();
					rest = this->parse_additive_expr();
					result = std::make_unique<greater_equal_expr>(std::move(result), std::move(rest));
					break;
				default:
					goto exit;
			}
		}

	exit:
		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_equality_expr() {
		std::unique_ptr<const expr> result = this->parse_ordering_expr();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_kind::equal_operator:
					this->ignore();
					rest = this->parse_ordering_expr();
					result = std::make_unique<equal_expr>(std::move(result), std::move(rest));
					break;
				case token_kind::not_equal_operator:
					this->ignore();
					rest = this->parse_ordering_expr();
					result = std::make_unique<not_equal_expr>(std::move(result), std::move(rest));
					break;
				default:
					goto exit;
			}
		}

	exit:
		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_logical_and_expr() {
		std::unique_ptr<const expr> result = this->parse_equality_expr();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_kind::logical_and_operator:
					this->ignore();
					rest = this->parse_equality_expr();
					result = std::make_unique<logical_and_expr>(std::move(result), std::move(rest));
					break;
				default:
					goto exit;
			}
		}

	exit:
		return std::move(result);
	}

	template <typename CharT, class Traits>
	std::unique_ptr<const expr>
	basic_parser<CharT, Traits>::parse_logical_or_expr() {
		std::unique_ptr<const expr> result = this->parse_logical_and_expr();

		while (!this->eof()) {
			std::unique_ptr<const expr> rest = nullptr;
			token_type& token = this->peek();

			switch (token.kind()) {
				case token_kind::logical_or_operator:
					this->ignore();
					rest = this->parse_logical_and_expr();
					result = std::make_unique<logical_or_expr>(std::move(result), std::move(rest));
					break;
				default:
					goto exit;
			}
		}

	exit:
		return std::move(result);
	}

	template <typename CharT, class Traits>
	void
	basic_parser<CharT, Traits>::report_error(const typename basic_parser<CharT, Traits>::error_type& error) {
		auto error_equal = [&error] (const error_type& i) {
			return i.code() == error.code() && i.extent() == error.extent();
		};
		if (std::none_of(this->errors().cbegin(), this->errors().cend(), error_equal)) {
			this->errors().push_back(error);
			throw error;
		}
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
