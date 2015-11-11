/**
 * @file		token.hpp
 * Contains type and function declarations for tokens.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_TOKEN_HPP
#define CALC_TOKEN_HPP

#include "config.hpp"

#include "parser_fwd.hpp"
#include "script.hpp"

namespace calc {
	template <typename CharT, class Traits = symbol_traits<CharT>>
	class basic_token;

	/// A token composed of @c char characters.
	typedef basic_token<char> token;

	/// A token composed of @c wchar_t characters.
	typedef basic_token<wchar_t> wtoken;

	/**
	 * Represents a token.
	 * @tparam CharT	The character type.
	 * @tparam Traits	The symbol traits type.
	 */
	template <typename CharT, class Traits>
	class basic_token {
		friend class basic_lexer<CharT, Traits>;
		friend class basic_parser<CharT, Traits>;

	public:
		typedef CharT char_type;
		typedef Traits traits_type;
		typedef typename Traits::string_type string_type;
		typedef typename Traits::string_view_type string_view_type;
		typedef basic_script_position<CharT, Traits> position_type;
		typedef basic_script_extent<CharT, Traits> extent_type;

		constexpr extent_type extent() const noexcept {
			return this->_extent;
		}

		constexpr token_kind kind() const noexcept {
			return this->_kind;
		}

		constexpr token_flags flags() const noexcept {
			return this->_flags;
		}

		constexpr explicit operator bool() const noexcept {
			return token_flags::has_error != (this->flags() & token_flags::has_error);
		}

		string_view_type text() const {
			return this->extent().text();
		}

	private:
		/// Maps token kind to default token flags.
		static constexpr token_flags _default_token_flags[21] = {
			token_flags::none,
			token_flags::none,
			token_flags::none,
			token_flags::none,
			token_flags::none,
			(token_flags::unary_operator | token_flags::right_associative | token_flags::unary_precedence) | (token_flags::binary_operator | token_flags::left_associative | token_flags::additive_precedence),
			(token_flags::unary_operator | token_flags::right_associative | token_flags::unary_precedence) | (token_flags::binary_operator | token_flags::left_associative | token_flags::additive_precedence),
			token_flags::binary_operator | token_flags::left_associative | token_flags::multiplicative_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::multiplicative_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::multiplicative_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::equality_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::equality_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::ordering_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::ordering_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::ordering_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::ordering_precedence,
			token_flags::unary_operator | token_flags::right_associative | token_flags::unary_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::logical_and_precedence,
			token_flags::binary_operator | token_flags::left_associative | token_flags::logical_or_precedence,
			token_flags::none,
			token_flags::none
		};

		extent_type _extent;
		token_kind _kind;
		token_flags _flags;

		/**
		 * Constructs a token of the given kind with the given flags.
		 * @param extent	The span of text in the script.
		 * @param kind		The kind of token.
		 * @param flags		The flags for the token.
		 */
		constexpr basic_token(const extent_type& extent,
		                      token_kind kind,
		                      token_flags flags) noexcept :
			_extent(extent), _kind(kind), _flags(flags)
		{}

		/**
		 * Constructs a token of the given kind with the default flags for
		 * that kind of token.
		 * @param extent	The span of text in the script.
		 * @param kind		The kind of token.
		 */
		constexpr basic_token(const extent_type& extent,
		                      token_kind kind) noexcept :
			basic_token(extent, kind, _default_token_flags[static_cast<int>(kind)])
		{}

		/**
		 * Constructs a token of unknown kind with the default flags for that
		 * kind of token.
		 * @param extent	The span of text in the script.
		 */
		constexpr explicit basic_token(const extent_type& extent) noexcept :
			basic_token(extent, token_kind::unknown)
		{}

		// doesn't compile with 'constexpr' specifier for some reason
		void flags(token_flags flags) noexcept {
			this->_flags = flags;
		}
	};
} // namespace calc

#include "token.ipp"

#endif // CALC_TOKEN_HPP
