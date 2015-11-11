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
#include <ostream>
#include <string>

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

	class type;
	class boolean_type;
	class integer_type;

	class value;
	class boolean_value;
	class integer_value;

	/**
	 * Represents an expression.
	 */
	class expr {
	public:
		virtual ~expr() = 0;
		virtual std::unique_ptr<class value> value() const = 0;
	};

	/**
	 * Represents an unary expression.
	 */
	class unary_expr : public expr {
	public:
		explicit unary_expr(const expr* operand1) noexcept;
		explicit unary_expr(std::unique_ptr<const expr>&& operand1) noexcept;
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
		binary_expr(const expr* operand1, const expr* operand2) noexcept;
		binary_expr(std::unique_ptr<const expr>&& operand1,
		            std::unique_ptr<const expr>&& operand2) noexcept;
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
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a negative (unary negation) expression.
	 */
	class negative_expr : public unary_expr {
	public:
		using unary_expr::unary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents an addition expression.
	 */
	class addition_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a subtraction expression.
	 */
	class subtraction_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a multiplication expression.
	 */
	class multiplication_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a division expression.
	 */
	class division_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a modulus expression.
	 */
	class modulus_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents an equality expression.
	 */
	class equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents an inequality expression.
	 */
	class not_equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a "less than" ordering expression.
	 */
	class less_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a "greater than" ordering expression.
	 */
	class greater_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a "less than or equal to" ordering expression.
	 */
	class less_equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a "greater than or equal to" ordering expression.
	 */
	class greater_equal_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a logical "not" (negation) expression.
	 */
	class logical_not_expr : public unary_expr {
	public:
		using unary_expr::unary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a logical "and" expression.
	 */
	class logical_and_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a logical "or" expression.
	 */
	class logical_or_expr : public binary_expr {
	public:
		using binary_expr::binary_expr;
		std::unique_ptr<class value> value() const;
	};

	/**
	 * Represents a boolean literal expression.
	 */
	class boolean : public expr {
	public:
		explicit boolean(bool v) noexcept;
		std::unique_ptr<class value> value() const;

	private:
		bool _value;
	};

	/**
	 * Represents an integer literal expression.
	 */
	class integer : public expr {
	public:
		explicit integer(std::int32_t v) noexcept;
		std::unique_ptr<class value> value() const;

	private:
		std::int32_t _value;
	};

	/**
	 * Represents a type.
	 */
	class type {
	public:
		type(const type&) = delete;
		virtual ~type();

		type& operator=(const type&) = delete;

		std::string name() const;

	protected:
		explicit type(const char* name);
		explicit type(const std::string& name);

	private:
		std::string _name;
	};

	bool operator==(const type& type1, const type& type2);
	bool operator!=(const type& type1, const type& type2);

	/**
	 * Represents a boolean type.
	 */
	class boolean_type : public type {
	public:
		/// The singleton instance of the boolean_type class.
		static const boolean_type instance;

		boolean_type(const boolean_type&) = delete;

		boolean_type& operator=(const boolean_type&) = delete;

	protected:
		boolean_type();
	};

	/**
	 * Represents an integer type.
	 */
	class integer_type : public type {
	public:
		/// The singleton instance of the integer_type class.
		static const integer_type instance;

		integer_type(const integer_type&) = delete;

		integer_type& operator=(const integer_type&) = delete;

	protected:
		integer_type();
	};

	/**
	 * Represents a value.
	 */
	class value {
	public:
		value(const value& other) noexcept;
		virtual ~value();

		const class type& type() const noexcept;

		bool is_equal(const value& other) const noexcept;

	protected:
		explicit value(const class type& type) noexcept;

		virtual bool do_is_equal(const value& other) const noexcept = 0;

	private:
		const class type& _type;
	};

	bool operator==(const value& value1, const value& value2);
	bool operator!=(const value& value1, const value& value2);

	/**
	 * Represents a boolean value.
	 */
	class boolean_value : public value {
	public:
		boolean_value(bool v = bool()) noexcept;
		boolean_value(const boolean_value& other) noexcept;

		boolean_value& operator=(bool v) noexcept;
		boolean_value& operator=(const boolean_value& other) noexcept;

		bool to_bool() const noexcept;
		operator bool() const noexcept;

	protected:
		virtual bool do_is_equal(const value& other) const noexcept;

	private:
		bool _data;
	};

	/**
	 * Represents an integer value.
	 */
	class integer_value : public value {
	public:
		integer_value(std::int32_t v = std::int32_t()) noexcept;
		integer_value(const integer_value& other) noexcept;

		integer_value& operator=(std::int32_t v) noexcept;
		integer_value& operator=(const integer_value& other) noexcept;

		#define DECLARE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(OP) \
			integer_value& operator OP##=(std::int32_t v) noexcept;

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

		integer_value& operator++() noexcept;
		integer_value operator++(int) noexcept;
		integer_value& operator--() noexcept;
		integer_value operator--(int) noexcept;

		std::int32_t to_int32() const noexcept;
		operator std::int32_t() const noexcept;

	protected:
		virtual bool do_is_equal(const value& other) const noexcept;

	private:
		std::int32_t _data;
	};

	template <typename CharT, class Traits>
	inline std::basic_ostream<CharT, Traits>&
	operator<<(std::basic_ostream<CharT, Traits>& out, const value& v) {
		if (v.type() == boolean_type::instance)
			return out << dynamic_cast<const boolean_value&>(v).to_bool();
		if (v.type() == integer_type::instance)
			return out << dynamic_cast<const integer_value&>(v).to_int32();
		return out;
	}
} // namespace calc

#endif // CALC_AST_HPP
