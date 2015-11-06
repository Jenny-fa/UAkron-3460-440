/**
 * @file		parser.cpp
 * Contains type and function definitions for parsing.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#include "parser.hpp"

namespace calc {
	// Explicit instantiations for the basic_lexer class template.
	template class basic_parser<char>;
	template class basic_parser<wchar_t>;
} // namespace calc
