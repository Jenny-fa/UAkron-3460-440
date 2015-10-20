/**
 * @file		token.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/7/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_TOKEN_IPP
#define CALC_TOKEN_IPP

namespace calc {
	template <typename CharT, class Traits>
	constexpr token_base::flags basic_token<CharT, Traits>::_default_token_flags[11];

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_token<char>;
	extern template class basic_token<wchar_t>;
} // namespace calc

#endif // CALC_TOKEN_IPP
