/**
 * @file		script.cpp
 * Contains type and function definitions for script positions and extents.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#include "script.hpp"

namespace calc {
	// Explicit instantiations for the basic_script_* class templates.
	template class basic_script_position_helper<char>;
	template class basic_script_position_helper<wchar_t>;
	template class basic_script_position<char>;
	template class basic_script_position<wchar_t>;
	template class basic_script_extent<char>;
	template class basic_script_extent<wchar_t>;

	template std::ostream& operator<<(std::ostream&, const script_position&);
	template std::ostream& operator<<(std::ostream&, const script_extent&);
	template std::wostream& operator<<(std::wostream&, const wscript_position&);
	template std::wostream& operator<<(std::wostream&, const wscript_extent&);
} // namespace calc
