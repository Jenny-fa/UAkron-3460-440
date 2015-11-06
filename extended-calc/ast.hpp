/**
 * @file		ast.hpp
 * Contains type declarations for an abstract syntax tree.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
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
	class positive_expr;
	class negative_expr;
	class addition_expr;
	class subtraction_expr;
	class multiplication_expr;
	class division_expr;
	class modulus_expr;
	class equal_expr;
	class not_equal_expr;
	class less_expr;
	class greater_expr;
	class less_equal_expr;
	class greater_equal_expr;
	class logical_not_expr;
	class logical_and_expr;
	class logical_or_expr;
	class boolean;
	class integer;

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
	 * Represents a positive (unary plus) expression.
	 */
	class positive_expr : public unary_expr {
	public:
		using unary_expr::unary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a negative (unary negation) expression.
	 */
	class negative_expr : public unary_expr {
	public:
		using unary_expr::unary_expr;
		std::intmax_t value() const;
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

	/**
	 * Represents an equality expression.
	 */
	class equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents an inequality expression.
	 */
	class not_equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a "less than" ordering expression.
	 */
	class less_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a "greater than" ordering expression.
	 */
	class greater_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a "less than or equal to" ordering expression.
	 */
	class less_equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a "greater than or equal to" ordering expression.
	 */
	class greater_equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a logical not (negation) expression.
	 */
	class logical_not_expr : public unary_expr {
	public:
		using unary_expr::unary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a logical "and" expression.
	 */
	class logical_and_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a logical "or" expression.
	 */
	class logical_or_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::intmax_t value() const;
	};

	/**
	 * Represents a boolean literal expression.
	 */
	class boolean : public expr {
	public:
		boolean(bool v = bool()) noexcept;

		std::intmax_t value() const noexcept;
		operator bool() const noexcept;

	private:
		bool _value;
	};

	/**
	 * Represents a signed integer literal expression.
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
} // namespace calc

#endif // CALC_AST_HPP
