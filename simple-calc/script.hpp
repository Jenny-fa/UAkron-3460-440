/**
 * @file		script.hpp
 * Contains type and function declarations for script positions and extents.
 *
 * @author		Jennifer Yao
 * @date		10/5/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_SCRIPT_HPP
#define CALC_SCRIPT_HPP

#include "config.hpp"

#include <ostream>
#include <vector>

#include "lexer_fwd.hpp"
#include "parser_fwd.hpp"
#include "symbol_traits.hpp"

namespace calc {
	template <typename CharT, class Traits = symbol_traits<CharT>>
	class basic_script_position_helper;

	/// A script position helper for @c char characters.
	typedef basic_script_position_helper<char> script_position_helper;

	/// A script position helper for @c wchar_t characters.
	typedef basic_script_position_helper<wchar_t> wscript_position_helper;

	template <typename CharT, class Traits = symbol_traits<CharT>>
	class basic_script_position;

	/// A script position for @c char characters.
	typedef basic_script_position<char> script_position;

	/// A script position for @c wchar_t characters.
	typedef basic_script_position<wchar_t> wscript_position;

	template <typename CharT, class Traits = symbol_traits<CharT>>
	class basic_script_extent;

	/// A script extent for @c char characters.
	typedef basic_script_extent<char> script_extent;

	/// A script extent for @c wchar_t characters.
	typedef basic_script_extent<wchar_t> wscript_extent;

	/**
	 * A helper class template for the basic_script_position and
	 * basic_script_extent class templates.
	 */
	template <typename CharT, class Traits>
	class basic_script_position_helper {
		friend class basic_lexer<CharT, Traits>;

	public:
		typedef CharT char_type;
		typedef Traits traits_type;
		typedef typename Traits::string_type string_type;
		typedef typename Traits::string_view_type string_view_type;

		basic_script_position_helper(const basic_script_position_helper&) = delete;

		const string_type& script() const noexcept {
			return this->_script;
		}

		std::size_t get_line_number(std::size_t offset) const;
		std::size_t get_column_number(std::size_t offset) const;
		string_view_type get_line(std::size_t line) const;

	private:
		string_type _script;
		std::vector<std::size_t> _line_start_map;

		basic_script_position_helper() : _script(), _line_start_map({0}) {
			// set initial capacity of script
			this->_script.reserve(31);
		}

		string_type& script() noexcept {
			return this->_script;
		}

		const std::vector<std::size_t>& line_start_map() const noexcept {
			return this->_line_start_map;
		}

		void add_line_start(std::size_t offset) {
			this->_line_start_map.push_back(offset);
		}
	};

	template <typename CharT, class Traits>
	inline bool
	operator==(const basic_script_position_helper<CharT, Traits>& helper1,
	           const basic_script_position_helper<CharT, Traits>& helper2)
	{
		return &helper1 == &helper2;
	}

	template <typename CharT, class Traits>
	inline bool
	operator!=(const basic_script_position_helper<CharT, Traits>& helper1,
	           const basic_script_position_helper<CharT, Traits>& helper2)
	{
		return &helper1 != &helper2;
	}

	/**
	 * Represents a single point in a script.
	 */
	template <typename CharT, class Traits>
	class basic_script_position {
		friend class basic_script_extent<CharT, Traits>;
		friend class basic_lexer<CharT, Traits>;
		friend class basic_parser<CharT, Traits>;

		template <typename CharT2, class Traits2>
		friend bool
		operator==(const basic_script_position<CharT2, Traits2>& position1,
	               const basic_script_position<CharT2, Traits2>& position2);

		template <typename CharT2, class Traits2>
		friend bool
		operator!=(const basic_script_position<CharT2, Traits2>& position1,
	               const basic_script_position<CharT2, Traits2>& position2);

	public:
		typedef CharT char_type;
		typedef Traits traits_type;
		typedef typename Traits::string_type string_type;
		typedef typename Traits::string_view_type string_view_type;
		typedef typename Traits::ostream_type ostream_type;

		constexpr basic_script_position(const basic_script_position& position) noexcept = default;

		constexpr std::size_t offset() const noexcept {
			return this->_offset;
		}

		std::size_t line_number() const;
		std::size_t column_number() const;
		string_view_type line() const;

	private:
		typedef basic_script_position_helper<CharT, Traits> position_helper_type;

		const position_helper_type& _position_helper;
		std::size_t _offset;

		constexpr basic_script_position(const position_helper_type& position_helper,
		                                std::size_t offset) noexcept :
			_position_helper(position_helper), _offset(offset)
		{}

		constexpr const position_helper_type& position_helper() const noexcept {
			return this->_position_helper;
		}
	};

	template <typename CharT, class Traits>
	inline bool
	operator==(const basic_script_position<CharT, Traits>& position1,
	           const basic_script_position<CharT, Traits>& position2)
	{
		return position1.position_helper() == position2.position_helper()
		       && position1.offset() == position2.offset();
	}

	template <typename CharT, class Traits>
	inline bool
	operator!=(const basic_script_position<CharT, Traits>& position1,
	           const basic_script_position<CharT, Traits>& position2)
	{
		return position1.position_helper() != position2.position_helper()
		       || position1.offset() != position2.offset();
	}

	template <typename CharT, class Traits, class STraits = typename Traits::char_traits_type>
	std::basic_ostream<CharT, STraits>&
	operator<<(std::basic_ostream<CharT, STraits>& out,
	           const basic_script_position<CharT, Traits>& position);

	/**
	 * Represents a span of text in a script.
	 */
	template <typename CharT, class Traits>
	class basic_script_extent {
		friend class basic_lexer<CharT, Traits>;
		friend class basic_parser<CharT, Traits>;

		template <typename CharT2, class Traits2>
		friend bool
		operator==(const basic_script_extent<CharT2, Traits2>& extent1,
	               const basic_script_extent<CharT2, Traits2>& extent2);

		template <typename CharT2, class Traits2>
		friend bool
		operator!=(const basic_script_extent<CharT2, Traits2>& extent1,
	               const basic_script_extent<CharT2, Traits2>& extent2);

	private:
		typedef basic_script_position_helper<CharT, Traits> position_helper_type;

		constexpr const position_helper_type& position_helper() const noexcept {
			return this->_position_helper;
		}

	public:
		typedef CharT char_type;
		typedef Traits traits_type;
		typedef typename Traits::string_type string_type;
		typedef typename Traits::string_view_type string_view_type;
		typedef basic_script_position<CharT, Traits> position_type;

		constexpr basic_script_extent(const basic_script_extent& extent) noexcept = default;

		constexpr std::size_t start_offset() const noexcept {
			return this->_start_offset;
		}

		constexpr std::size_t end_offset() const noexcept {
			return this->_end_offset;
		}

		constexpr position_type start_position() const noexcept;
		constexpr position_type end_position() const noexcept;

		std::size_t start_line_number() const;
		std::size_t start_column_number() const;
		std::size_t end_line_number() const;
		std::size_t end_column_number() const;

		string_view_type text() const;

	private:
		const position_helper_type& _position_helper;
		std::size_t _start_offset;
		std::size_t _end_offset;

		constexpr basic_script_extent(const position_helper_type& position_helper,
		                              std::size_t start_offset,
		                              std::size_t end_offset) noexcept :
			_position_helper(position_helper),
			_start_offset(start_offset),
			_end_offset(end_offset)
		{}
	};

	template <typename CharT, class Traits>
	inline bool
	operator==(const basic_script_extent<CharT, Traits>& extent1,
	           const basic_script_extent<CharT, Traits>& extent2)
	{
		return extent1.position_helper() == extent2.position_helper()
		       && extent1.start_offset() == extent2.start_offset()
		       && extent1.end_offset() == extent2.end_offset();
	}

	template <typename CharT, class Traits>
	inline bool
	operator!=(const basic_script_extent<CharT, Traits>& extent1,
	           const basic_script_extent<CharT, Traits>& extent2)
	{
		return extent1.position_helper() != extent2.position_helper()
		       || extent1.start_offset() != extent2.start_offset()
		       || extent1.end_offset() != extent2.end_offset();
	}

	template <typename CharT, class Traits, class STraits = typename Traits::char_traits_type>
	std::basic_ostream<CharT, STraits>&
	operator<<(std::basic_ostream<CharT, STraits>& out,
	           const basic_script_extent<CharT, Traits>& extent);
} // namespace calc

#include "script.ipp"

#endif // CALC_SCRIPT_HPP
