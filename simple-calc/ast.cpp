/**
 * @file		ast.cpp
 * Contains type definitions for an abstract syntax tree.
 *
 * @author		Jennifer Yao
 * @date		9/19/2015
 * @copyright	All rights reserved.
 */

#include <stdexcept>

#include "ast.hpp"

namespace calc {
	expr::~expr() {}

	unary_expr::unary_expr(const expr* operand1) :
		_operand(operand1)
	{
		if (!operand1)
			throw std::invalid_argument("calc::unary_expr::unary_expr");
	}

	unary_expr::~unary_expr() {}

	const expr* unary_expr::operand() const noexcept {
		return this->_operand.get();
	}

	binary_expr::binary_expr(const expr* operand1, const expr* operand2) :
		_left_operand(operand1),
		_right_operand(operand2)
	{
		if (!operand1 || !operand2)
			throw std::invalid_argument("calc::unary_expr::unary_expr");
	}

	binary_expr::~binary_expr() {}

	const expr* binary_expr::left_operand() const noexcept {
		return this->_left_operand.get();
	}

	const expr* binary_expr::right_operand() const noexcept {
		return this->_right_operand.get();
	}

	std::intmax_t addition_expr::eval() const {
		return this->left_operand()->eval() + this->right_operand()->eval();
	}

	std::intmax_t subtraction_expr::eval() const {
		return this->left_operand()->eval() - this->right_operand()->eval();
	}

	std::intmax_t multiplication_expr::eval() const {
		return this->left_operand()->eval() * this->right_operand()->eval();
	}

	std::intmax_t division_expr::eval() const {
		const std::intmax_t rhs_value = this->right_operand()->eval();
		if (rhs_value == 0)
			throw std::domain_error("calc::division_expr::eval");
		return this->left_operand()->eval() / rhs_value;
	}

	std::intmax_t modulus_expr::eval() const {
		const std::intmax_t rhs_value = this->right_operand()->eval();
		if (rhs_value == 0)
			throw std::domain_error("calc::modulus_expr::eval");
		return this->left_operand()->eval() % rhs_value;
	}

#if !MOAR_DIGITS
	digit::digit(int v) : _value(v) {
		if (v < 0 || v > 9)
			throw std::invalid_argument("calc::digit::digit");
	}

	digit& digit::operator=(int v) {
		if (v < 0 || v > 9)
			throw std::invalid_argument("calc::digit::operator=");
		this->_value = v;
		return *this;
	}

	int digit::value() const noexcept {
		return this->_value;
	}

	digit::operator int() const noexcept {
		return this->_value;
	}

	std::intmax_t digit::eval() const noexcept {
		return this->value();
	}
#else
	integer::integer(std::intmax_t v) noexcept : _value(v) {}

	#define DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(OP) \
		integer& integer::operator OP##=(std::intmax_t v) noexcept { \
			*this = std::intmax_t(*this) OP v; \
			return *this; \
		}

	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(+)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(-)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(*)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(/)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(%)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(&)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(|)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(^)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(<<)
	DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(>>)

	#undef DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR

	integer& integer::operator++() noexcept {
		++(this->_value);
		return *this;
	}

	integer integer::operator++(int) noexcept {
		integer result(*this);
		++(*this);
		return result;
	}

	integer& integer::operator--() noexcept {
		--(this->_value);
		return *this;
	}

	integer integer::operator--(int) noexcept {
		integer result(*this);
		--(*this);
		return result;
	}

	std::intmax_t integer::value() const noexcept {
		return this->_value;
	}

	integer::operator std::intmax_t() const noexcept {
		return this->_value;
	}

	std::intmax_t integer::eval() const noexcept {
		return this->value();
	}
#endif
} // namespace calc
