/**
 * @file		token.hpp
 * Contains type and function declarations for tokens.
 *
 * @author		Jennifer Yao
 * @date		9/25/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_TOKEN_HPP
#define CALC_TOKEN_HPP

#include "config.hpp"

#include "parser_fwd.hpp"
#include "script.hpp"

namespace calc {
	template <typename CharT, class Traits = symbol_traits<CharT> >
	class basic_token;

	/// A token composed of @c char characters.
	typedef basic_token<char> token;

	/// A token composed of @c wchar_t characters.
	typedef basic_token<wchar_t> wtoken;

	/**
	 * Provides types which are inherited and used by the basic_token class
	 * template.
	 */
	class token_base {
	public:
		/// The kinds of token that are recognized by the calculator.
		enum kind {
			unknown,
			eof,
			newline,
#if !MOAR_DIGITS
			digit,
#else
			integer,
#endif
			addition_operator,
			subtraction_operator,
			multiplication_operator,
			division_operator,
			modulus_operator,
			left_parenthesis,
			right_parenthesis
		};

		/// Flags that specify additional information about a given token.
		enum flags {
			none = 0,
			has_error = 1 << 0,
			unary_operator = 1 << 1,
			binary_operator = 2 << 1,
			operator_arity_mask = ((1 << 2) - 1) << 1,
			left_associative = 0 << 3,
			right_associative = 1 << 3,
			operator_associativity_mask = ((1 << 1) - 1) << 3,
			multiplication_precedence = 0 << 4,
			addition_precedence = 1 << 4,
			operator_precedence_mask = ((1 << 4) - 1) << 4
		};
	};

	/**
	 * Represents a token.
	 * @tparam CharT	The character type.
	 * @tparam Traits	The symbol traits type.
	 */
	template <typename CharT, class Traits>
	class basic_token : public token_base {
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

		constexpr token_base::kind kind() const noexcept {
			return this->_kind;
		}

		constexpr token_base::flags flags() const noexcept {
			return this->_flags;
		}

		constexpr explicit operator bool() const noexcept {
			return token_base::flags::has_error != (this->flags() & token_base::flags::has_error);
		}

		string_view_type text() const {
			return this->extent().text();
		}

	private:
		/// Maps token kind to default token flags.
		static constexpr token_base::flags _default_token_flags[11] = {
			none,
			none,
			none,
			none,
			token_base::flags(binary_operator | left_associative | addition_precedence),
			token_base::flags(binary_operator | left_associative | addition_precedence),
			token_base::flags(binary_operator | left_associative | multiplication_precedence),
			token_base::flags(binary_operator | left_associative | multiplication_precedence),
			token_base::flags(binary_operator | left_associative | multiplication_precedence),
			none,
			none
		};

		extent_type _extent;
		token_base::kind _kind;
		token_base::flags _flags;

		/**
		 * Constructs a token of the given kind with the given flags.
		 * @param extent	The span of text in the script.
		 * @param kind		The kind of token.
		 * @param flags		The flags for the token.
		 */
		constexpr basic_token(const extent_type& extent,
		                      token_base::kind kind,
		                      token_base::flags flags) noexcept :
			_extent(extent), _kind(kind), _flags(flags)
		{}

		/**
		 * Constructs a token of the given kind with the default flags for
		 * that kind of token.
		 * @param extent	The span of text in the script.
		 * @param kind		The kind of token.
		 */
		constexpr basic_token(const extent_type& extent,
			                  token_base::kind kind) noexcept :
			basic_token(extent, kind, _default_token_flags[kind])
		{}

		/**
		 * Constructs a token of unknown kind with the default flags for that
		 * kind of token.
		 * @param extent	The span of text in the script.
		 */
		constexpr explicit basic_token(const extent_type& extent) noexcept :
			basic_token(extent, token_base::kind::unknown)
		{}

		// doesn't compile with 'constexpr' specifier for some reason
		void flags(token_base::flags flags) noexcept {
			this->_flags = flags;
		}
	};
} // namespace calc

#include "token.ipp"

#endif // CALC_TOKEN_HPP
