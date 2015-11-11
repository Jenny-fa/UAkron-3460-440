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
	template <typename CharT>
	constexpr std::size_t symbol_traits<CharT>::npos;

	template <typename CharT>
	typename symbol_traits<CharT>::string_type
	symbol_traits<CharT>::widen(const std::string& str) const {
		const std::ctype<CharT>& ctype_facet = std::use_facet<std::ctype<CharT>>(this->_locale);
		const std::size_t n = str.size();

		char_type* wstr = new char_type[n];
		ctype_facet.widen(str.c_str(), str.c_str() + n, wstr);
		string_type result(wstr, n);
		delete[] wstr;

		return result;
	}

	template <typename CharT>
	bool
	symbol_traits<CharT>::bool_value(const typename symbol_traits<CharT>::string_type& str, std::size_t* idx) const {
		const string_type& true_name = this->true_name();
		const string_type& false_name = this->false_name();

		std::size_t start_index = 0;

		while (start_index < str.size()
		       && std::isspace(str[start_index], this->_locale))
			start_index++;

		bool result;

		if (str.find(true_name, start_index) == start_index) {
			if (idx)
				*idx = start_index + true_name.size();
			result = true;
		}
		else if (str.find(false_name, start_index) == start_index) {
			if (idx)
				*idx = start_index + false_name.size();
			result = false;
		}
		else
			throw std::invalid_argument("calc::symbol_traits::bool_value");

		return result;
	}

	template <typename CharT>
	void symbol_traits<CharT>::init() {
		const std::numpunct<CharT>& numpunct_facet = std::use_facet<std::numpunct<CharT>>(this->_locale);

		for (std::size_t i = 0; i < 3; i++)
			this->_newlines[i] = this->widen(symbol_base::newlines[i]);

		this->_true_name = numpunct_facet.truename();
		this->_false_name = numpunct_facet.falsename();

		for (const auto& i : symbol_base::operator_table)
			this->_operator_table[i.first] = this->widen(i.second);
	}

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class symbol_traits<char>;
	extern template class symbol_traits<wchar_t>;
} // namespace calc

#endif // CALC_SYMBOL_TRAITS_IPP
