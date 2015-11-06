/**
 * @file		constants.hpp
 * Contains constant definitions.
 *
 * @author		Jennifer Yao
 * @date		11/4/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_CONSTANTS_HPP
#define CALC_CONSTANTS_HPP

#include "config.hpp"

namespace calc {
	/// Identifies the kind of parsing error.
	enum class error_id {
		unknown_token,
		unexpected_token,
		missing_end_parenthesis
	};

	/// The kinds of token that are recognized by the calculator.
	enum class token_kind {
		unknown,
		eof,
		newline,
		boolean,
		integer,
		positive_or_addition_operator,
		negative_or_subtraction_operator,
		multiplication_operator,
		division_operator,
		modulus_operator,
		equal_operator,
		not_equal_operator,
		less_operator,
		greater_operator,
		less_equal_operator,
		greater_equal_operator,
		logical_not_operator,
		logical_and_operator,
		logical_or_operator,
		left_parenthesis,
		right_parenthesis
	};

	/// Flags that specify additional information about a given token.
	enum class token_flags : unsigned int {
		none = 0,
		has_error = 1 << 0,
		unary_operator = 1 << 1,
		binary_operator = 1 << 2,
		operator_arity_mask = unary_operator | binary_operator,
		left_associative = 1 << 3,
		right_associative = 1 << 4,
		operator_associativity_mask = left_associative | right_associative,
		unary_precedence = 1 << 5,
		multiplicative_precedence = 1 << 6,
		additive_precedence = 2 << 6,
		ordering_precedence = 3 << 6,
		equality_precedence = 4 << 6,
		logical_and_precedence = 5 << 6,
		logical_or_precedence = 6 << 6,
		unary_operator_precedence_mask = ((1 << 1) - 1) << 5,
		binary_operator_precedence_mask = ((1 << 4) - 1) << 6,
		operator_precedence_mask = unary_operator_precedence_mask | binary_operator_precedence_mask
	};

	constexpr inline token_flags operator&(token_flags a, token_flags b) {
		return token_flags(static_cast<unsigned int>(a)
			& static_cast<unsigned int>(b));
	}

	constexpr inline token_flags operator|(token_flags a, token_flags b) {
		return token_flags(static_cast<unsigned int>(a)
			| static_cast<unsigned int>(b));
	}

	constexpr inline token_flags operator^(token_flags a, token_flags b) {
		return token_flags(static_cast<unsigned int>(a)
			^ static_cast<unsigned int>(b));
	}

	constexpr inline token_flags operator~(token_flags a) {
		return token_flags(~static_cast<unsigned int>(a));
	}

	inline token_flags& operator&=(token_flags& a, token_flags b) {
		return a = a & b;
	}

	inline token_flags& operator|=(token_flags& a, token_flags b) {
		return a = a | b;
	}

	inline token_flags& operator^=(token_flags& a, token_flags b) {
		return a = a ^ b;
	}
} // namespace calc

#endif // CALC_CONSTANTS_HPP
