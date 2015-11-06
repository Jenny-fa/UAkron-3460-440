/**
 * @file		symbol_traits.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_SYMBOL_TRAITS_IPP
#define CALC_SYMBOL_TRAITS_IPP

namespace calc {
	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class symbol_traits<char>;
	extern template class symbol_traits<wchar_t>;
} // namespace calc

#endif // CALC_SYMBOL_TRAITS_IPP
