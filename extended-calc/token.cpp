/**
 * @file		token.cpp
 * Contains type and function definitions for tokens.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#include "token.hpp"

namespace calc {
	// Explicit instantiations for the basic_token class template.
	template class basic_token<char>;
	template class basic_token<wchar_t>;
} // namespace calc
