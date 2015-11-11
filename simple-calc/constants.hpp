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
#if !MOAR_DIGITS
		digit,
#else
		integer,
#endif
		addition_operator,
		subtraction_operator,
		multiplication_operator,
		division_operator,
		modulus_operator,
		left_parenthesis,
		right_parenthesis
	};

	/// Flags that specify additional information about a given token.
	enum class token_flags : unsigned int {
		none = 0,
		has_error = 1 << 0,
		unary_operator = 1 << 1,
		binary_operator = 2 << 1,
		operator_arity_mask = ((1 << 2) - 1) << 1,
		left_associative = 0 << 3,
		right_associative = 1 << 3,
		operator_associativity_mask = ((1 << 1) - 1) << 3,
		multiplicative_precedence = 0 << 4,
		additive_precedence = 1 << 4,
		operator_precedence_mask = ((1 << 4) - 1) << 4
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
