/**
 * @file		ast.cpp
 * Contains type definitions for an abstract syntax tree.
 *
 * @author		Jennifer Yao
 * @date		9/19/2015
 * @copyright	All rights reserved.
 */

#include <cassert>
#include <stdexcept>

#include "ast.hpp"

namespace calc {
	expr::~expr() {}

	unary_expr::unary_expr(const expr* operand1) noexcept :
		unary_expr(std::unique_ptr<const expr>(operand1))
	{}

	unary_expr::unary_expr(std::unique_ptr<const expr>&& operand1) noexcept :
		_operand(std::move(operand1))
	{
		assert(this->_operand);
	}

	unary_expr::~unary_expr() {}

	const expr* unary_expr::operand() const noexcept {
		return this->_operand.get();
	}

	binary_expr::binary_expr(const expr* operand1,
	                         const expr* operand2) noexcept :
		binary_expr(std::unique_ptr<const expr>(operand1),
		            std::unique_ptr<const expr>(operand2))
	{}

	binary_expr::binary_expr(std::unique_ptr<const expr>&& operand1,
	                         std::unique_ptr<const expr>&& operand2) noexcept :
		_left_operand(std::move(operand1)),
		_right_operand(std::move(operand2))
	{
		assert(this->_left_operand && this->_right_operand);
	}

	binary_expr::~binary_expr() {}

	const expr* binary_expr::left_operand() const noexcept {
		return this->_left_operand.get();
	}

	const expr* binary_expr::right_operand() const noexcept {
		return this->_right_operand.get();
	}

	std::intmax_t addition_expr::value() const {
		return this->left_operand()->value() + this->right_operand()->value();
	}

	std::intmax_t subtraction_expr::value() const {
		return this->left_operand()->value() - this->right_operand()->value();
	}

	std::intmax_t multiplication_expr::value() const {
		return this->left_operand()->value() * this->right_operand()->value();
	}

	std::intmax_t division_expr::value() const {
		const std::intmax_t rhs_value = this->right_operand()->value();
		if (!rhs_value)
			throw std::domain_error("calc::division_expr::value");
		return this->left_operand()->value() / rhs_value;
	}

	std::intmax_t modulus_expr::value() const {
		const std::intmax_t rhs_value = this->right_operand()->value();
		if (!rhs_value)
			throw std::domain_error("calc::modulus_expr::value");
		return this->left_operand()->value() % rhs_value;
	}

#if !MOAR_DIGITS
	digit::digit(int v) : _value(v) {
		assert(v >= 0 && v <= 9);
	}

	digit& digit::operator=(int v) {
		assert(v >= 0 && v <= 9);
		this->_value = v;
		return *this;
	}

	std::intmax_t digit::value() const noexcept {
		return this->_value;
	}

	digit::operator int() const noexcept {
		return this->_value;
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
#endif
} // namespace calc
