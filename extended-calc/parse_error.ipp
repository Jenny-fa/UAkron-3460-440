/**
 * @file		parse_error.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_PARSE_ERROR_IPP
#define CALC_PARSE_ERROR_IPP

namespace calc {
	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_parse_error<char>;
	extern template class basic_parse_error<wchar_t>;
} // namespace calc

#endif // CALC_PARSE_ERROR_IPP
