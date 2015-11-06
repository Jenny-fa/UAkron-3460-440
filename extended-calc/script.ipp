/**
 * @file		script.ipp
 * Contains template definitions and explicit template instantiation
 * declarations.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_SCRIPT_IPP
#define CALC_SCRIPT_IPP

#include <algorithm>
#include <iterator>

namespace calc {
	template <typename CharT, class Traits>
	std::size_t
	basic_script_position_helper<CharT, Traits>::get_line_number(std::size_t offset) const {
		std::vector<std::size_t>::const_iterator i = std::upper_bound(this->line_start_map().cbegin(), this->line_start_map().cend(), offset);
		return std::distance(this->line_start_map().cbegin(), i);
	}

	template <typename CharT, class Traits>
	std::size_t
	basic_script_position_helper<CharT, Traits>::get_column_number(std::size_t offset) const {
		return offset - this->line_start_map()[this->get_line_number(offset) - 1] + 1;
	}

	template <typename CharT, class Traits>
	typename basic_script_position_helper<CharT, Traits>::string_view_type
	basic_script_position_helper<CharT, Traits>::get_line(std::size_t line) const {
		std::size_t offset = this->line_start_map()[line - 1];
		string_view_type script_view(this->script());
		if (line < this->line_start_map().size()) {
			std::size_t len = this->line_start_map()[line] - offset;
			return script_view.substr(offset, len);
		}
		return script_view.substr(offset);
	}

	template <typename CharT, class Traits>
	std::size_t
	basic_script_position<CharT, Traits>::line_number() const {
		return this->position_helper().get_line_number(this->offset());
	}

	template <typename CharT, class Traits>
	std::size_t
	basic_script_position<CharT, Traits>::column_number() const {
		return this->position_helper().get_column_number(this->offset());
	}

	template <typename CharT, class Traits>
	typename basic_script_position<CharT, Traits>::string_view_type
	basic_script_position<CharT, Traits>::line() const {
		return this->position_helper().get_line(this->offset());
	}

	template <typename CharT, class Traits>
	constexpr typename basic_script_extent<CharT, Traits>::position_type
	basic_script_extent<CharT, Traits>::start_position() const noexcept {
		return position_type(this->position_helper(), this->start_offset());
	}

	template <typename CharT, class Traits>
	constexpr typename basic_script_extent<CharT, Traits>::position_type
	basic_script_extent<CharT, Traits>::end_position() const noexcept {
		return position_type(this->position_helper(), this->end_offset());
	}

	template <typename CharT, class Traits>
	std::size_t
	basic_script_extent<CharT, Traits>::start_line_number() const {
		return this->position_helper().get_line_number(this->start_offset());
	}

	template <typename CharT, class Traits>
	std::size_t
	basic_script_extent<CharT, Traits>::start_column_number() const {
		return this->position_helper().get_column_number(this->start_offset());
	}

	template <typename CharT, class Traits>
	std::size_t
	basic_script_extent<CharT, Traits>::end_line_number() const {
		return this->position_helper().get_line_number(this->end_offset());
	}

	template <typename CharT, class Traits>
	std::size_t
	basic_script_extent<CharT, Traits>::end_column_number() const {
		return this->position_helper().get_column_number(this->end_offset());
	}

	template <typename CharT, class Traits>
	typename basic_script_extent<CharT, Traits>::string_view_type
	basic_script_extent<CharT, Traits>::text() const {
		if (this->start_offset() > this->position_helper().script().size())
			return string_view_type();
		string_view_type script_view(this->position_helper().script());
		if (this->end_offset() > this->position_helper().script().size())
			return script_view.substr(this->start_offset());
		return script_view.substr(this->start_offset(), this->end_offset() - this->start_offset());
	}

	template <typename CharT, class Traits, class STraits>
	std::basic_ostream<CharT, STraits>&
	operator<<(std::basic_ostream<CharT, STraits>& out,
	           const basic_script_position<CharT, Traits>& position)
	{
		const CharT left_brace = out.widen('{');
		const CharT right_brace = out.widen('}');
		const CharT comma = out.widen(',');
		const CharT space = out.widen(' ');
		return out << left_brace << position.line_number() << comma << space
		           << position.column_number() << right_brace;
	}

	template <typename CharT, class Traits, class STraits>
	std::basic_ostream<CharT, STraits>&
	operator<<(std::basic_ostream<CharT, STraits>& out,
	           const basic_script_extent<CharT, Traits>& extent)
	{
		const CharT left_brace = out.widen('{');
		const CharT right_brace = out.widen('}');
		const CharT comma = out.widen(',');
		const CharT space = out.widen(' ');
		return out << left_brace << extent.start_position() << comma << space
		           << extent.end_position() << right_brace;
	}

	// Inhibit implicit instantiations for required instantiations, which are
	// defined via explicit instantiations elsewhere.
	extern template class basic_script_position_helper<char>;
	extern template class basic_script_position_helper<wchar_t>;
	extern template class basic_script_position<char>;
	extern template class basic_script_position<wchar_t>;
	extern template class basic_script_extent<char>;
	extern template class basic_script_extent<wchar_t>;

	extern template std::ostream& operator<<(std::ostream&, const script_position&);
	extern template std::ostream& operator<<(std::ostream&, const script_extent&);
	extern template std::wostream& operator<<(std::wostream&, const wscript_position&);
	extern template std::wostream& operator<<(std::wostream&, const wscript_extent&);
} // namespace calc

#endif // CALC_SCRIPT_IPP
