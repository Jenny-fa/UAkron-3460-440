/**
 * @file		cli.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/14/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_CLI_IPP
#define CALC_CLI_IPP

namespace calc {
	template <typename CharT, class Traits>
	inline void report_error(const basic_parse_error<CharT, Traits>& error) {
		report_error("syntax error: %s", error.what());
	}

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template void report_error(const parse_error&);
	extern template void report_error(const wparse_error&);
} // namespace calc

#endif // CALC_CLI_IPP
