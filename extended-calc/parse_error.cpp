/**
 * @file		parse_error.cpp
 * Contains type and function definitions for parsing errors.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#include "parse_error.hpp"

namespace calc {
	// Explicit instantiations for the basic_parse_error class template.
	template class basic_parse_error<char>;
	template class basic_parse_error<wchar_t>;
} // namespace calc
