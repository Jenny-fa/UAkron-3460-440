/**
 * @file		ast.hpp
 * Contains type declarations for an abstract syntax tree.
 *
 * @author		Jennifer Yao
 * @date		9/19/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_AST_HPP
#define CALC_AST_HPP

#include "config.hpp"

#include <cstdint>
#include <memory>

namespace calc {
	class expr;
	class unary_expr;
	class binary_expr;
	class addition_expr;
	class subtraction_expr;
	class multiplication_expr;
	class division_expr;
	class modulus_expr;
#if !MOAR_DIGITS
	class digit;
#else
	class integer;
#endif

	/**
	 * Represents an expression.
	 */
	class expr {
	public:
		virtual ~expr() = 0;
		virtual std::intmax_t value() const = 0;
	};

	/**
	 * Represents an unary expression.
	 */
	class unary_expr : public expr {
	public:
		explicit unary_expr(const expr* operand1);
		explicit unary_expr(std::unique_ptr<const expr>&& operand1);
		virtual ~unary_expr() = 0;

		const expr* operand() const noexcept;

	private:
		std::unique_ptr<const expr> _operand;
	};

	/**
	 * Represents a binary expression.
	 */
	class binary_expr : public expr {
	public:
		binary_expr(const expr* operand1, const expr* operand2);
		binary_expr(std::unique_ptr<const expr>&& operand1,
		            std::unique_ptr<const expr>&& operand2);
		virtual ~binary_expr() = 0;

		const expr* left_operand() const noexcept;
		const expr* right_operand() const noexcept;

	private:
		std::unique_ptr<const expr> _left_operand;
		std::unique_ptr<const expr> _right_operand;
	};

	/**
	 * Represents an addition expression.
	 */
	class addition_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a subtraction expression.
	 */
	class subtraction_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a multiplication expression.
	 */
	class multiplication_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a division expression.
	 */
	class division_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a modulus expression.
	 */
	class modulus_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

#if !MOAR_DIGITS
	/**
	 * Represents a decimal digit.
	 */
	class digit : public expr {
	public:
		digit(int v = int());

		digit& operator=(int v);

		std::intmax_t value() const noexcept;
		operator std::intmax_t() const noexcept;

	private:
		int _value;
	};
#else
	/**
	 * Represents a signed integral number.
	 */
	class integer : public expr {
	public:
		integer(std::intmax_t v = std::intmax_t()) noexcept;

		#define DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(OP) \
			integer& operator OP##=(std::intmax_t v) noexcept;

		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(+)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(-)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(*)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(/)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(%)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(&)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(|)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(^)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(<<)
		DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(>>)

		#undef DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR

		integer& operator++() noexcept;
		integer operator++(int) noexcept;
		integer& operator--() noexcept;
		integer operator--(int) noexcept;

		std::intmax_t value() const noexcept;
		operator std::intmax_t() const noexcept;

	private:
		std::intmax_t _value;
	};
#endif
} // namespace calc

#endif // CALC_AST_HPP
