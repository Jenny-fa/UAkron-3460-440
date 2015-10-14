/**
 * @file		lexer.cpp
 * Contains type and function definitions for lexing.
 *
 * @author		Jennifer Yao
 * @date		9/25/2015
 * @copyright	All rights reserved.
 */

#include "lexer.hpp"

namespace calc {
	// Explicit instantiations for the basic_lexer class template.
	template class basic_lexer<char>;
	template class basic_lexer<wchar_t>;
} // namespace calc
