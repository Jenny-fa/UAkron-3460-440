/**
 * @file		symbol_traits.cpp
 * I don't even know anymore.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#include "symbol_traits.hpp"

namespace calc {
	const std::string symbol_base::newlines[3] = {"\n", "\r\n", "\r"};
	const std::map<token_kind, std::string> symbol_base::operator_table = {
		{token_kind::positive_or_addition_operator, "+"},
		{token_kind::negative_or_subtraction_operator, "-"},
		{token_kind::multiplication_operator, "*"},
		{token_kind::division_operator, "/"},
		{token_kind::modulus_operator, "%"},
		{token_kind::equal_operator, "=="},
		{token_kind::not_equal_operator, "!="},
		{token_kind::less_operator, "<"},
		{token_kind::greater_operator, ">"},
		{token_kind::less_equal_operator, "<="},
		{token_kind::greater_equal_operator, ">="},
		{token_kind::logical_not_operator, "!"},
		{token_kind::logical_and_operator, "&&"},
		{token_kind::logical_or_operator, "||"},
		{token_kind::left_parenthesis, "("},
		{token_kind::right_parenthesis, ")"}
	};

	// Explicit instantiations for the symbol_traits class template.
	template class symbol_traits<char>;
	template class symbol_traits<wchar_t>;
} // namespace calc
