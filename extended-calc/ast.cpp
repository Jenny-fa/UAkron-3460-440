/**
 * @file		ast.cpp
 * Contains type definitions for an abstract syntax tree.
 *
 * @author		Jennifer Yao
 * @date		10/26/2015
 * @copyright	All rights reserved.
 */

#include <cassert>
#include <stdexcept>
#include <typeinfo>

#include "ast.hpp"

namespace calc {
	// -----------------------------------------------------------------------
	// Expressions
	// -----------------------------------------------------------------------

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

	std::unique_ptr<class value> positive_expr::value() const {
		std::unique_ptr<class value> v = this->operand()->value();
		integer_value* integer_v = dynamic_cast<integer_value*>(v.get());

		if (!integer_v)
			throw std::invalid_argument("calc::positive_expr::value");

		return std::make_unique<integer_value>(+integer_v->to_int32());
	}

	std::unique_ptr<class value> negative_expr::value() const {
		std::unique_ptr<class value> v = this->operand()->value();
		integer_value* integer_v = dynamic_cast<integer_value*>(v.get());

		if (!integer_v)
			throw std::invalid_argument("calc::negative_expr::value");

		return std::make_unique<integer_value>(-integer_v->to_int32());
	}

	std::unique_ptr<class value> addition_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::addition_expr::value");

		return std::make_unique<integer_value>(left_integer_v->to_int32()
		                                       + right_integer_v->to_int32());
	}

	std::unique_ptr<class value> subtraction_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::subtraction_expr::value");

		return std::make_unique<integer_value>(left_integer_v->to_int32()
		                                       - right_integer_v->to_int32());
	}

	std::unique_ptr<class value> multiplication_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::multiplication_expr::value");

		return std::make_unique<integer_value>(left_integer_v->to_int32()
		                                       * right_integer_v->to_int32());
	}

	std::unique_ptr<class value> division_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::division_expr::value");
		if (right_integer_v->to_int32() == 0)
			throw std::domain_error("calc::division_expr::value");

		return std::make_unique<integer_value>(left_integer_v->to_int32()
		                                       / right_integer_v->to_int32());
	}

	std::unique_ptr<class value> modulus_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::modulus_expr::value");
		if (right_integer_v->to_int32() == 0)
			throw std::domain_error("calc::modulus_expr::value");

		return std::make_unique<integer_value>(left_integer_v->to_int32()
		                                       % right_integer_v->to_int32());
	}

	std::unique_ptr<class value> equal_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();

		if (typeid(*left_v) != typeid(*right_v))
			throw std::invalid_argument("calc::equal_expr::value");

		return std::make_unique<boolean_value>(left_v->is_equal(*right_v));
	}

	std::unique_ptr<class value> not_equal_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();

		if (typeid(*left_v) != typeid(*right_v))
			throw std::invalid_argument("calc::not_equal_expr::value");

		return std::make_unique<boolean_value>(!left_v->is_equal(*right_v));
	}

	std::unique_ptr<class value> less_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::less_expr::value");

		return std::make_unique<boolean_value>(left_integer_v->to_int32()
		                                       < right_integer_v->to_int32());
	}

	std::unique_ptr<class value> greater_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::greater_expr::value");

		return std::make_unique<boolean_value>(left_integer_v->to_int32()
		                                       > right_integer_v->to_int32());
	}

	std::unique_ptr<class value> less_equal_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::less_equal_expr::value");

		return std::make_unique<boolean_value>(left_integer_v->to_int32()
		                                       <= right_integer_v->to_int32());
	}

	std::unique_ptr<class value> greater_equal_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		integer_value* left_integer_v = dynamic_cast<integer_value*>(left_v.get());
		integer_value* right_integer_v = dynamic_cast<integer_value*>(right_v.get());

		if (!left_integer_v || !right_integer_v)
			throw std::invalid_argument("calc::greater_expr::value");

		return std::make_unique<boolean_value>(left_integer_v->to_int32()
		                                       <= right_integer_v->to_int32());
	}

	std::unique_ptr<class value> logical_not_expr::value() const {
		std::unique_ptr<class value> v = this->operand()->value();
		boolean_value* boolean_v = dynamic_cast<boolean_value*>(v.get());

		if (!boolean_v)
			throw std::invalid_argument("calc::logical_not_expr::value");

		return std::make_unique<boolean_value>(!boolean_v->to_bool());
	}

	std::unique_ptr<class value> logical_and_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		boolean_value* left_boolean_v = dynamic_cast<boolean_value*>(left_v.get());
		boolean_value* right_boolean_v = dynamic_cast<boolean_value*>(right_v.get());

		if (!left_boolean_v || !right_boolean_v)
			throw std::invalid_argument("calc::logical_and_expr::value");

		return std::make_unique<boolean_value>(left_boolean_v->to_bool()
		                                       && right_boolean_v->to_bool());
	}

	std::unique_ptr<class value> logical_or_expr::value() const {
		std::unique_ptr<class value> left_v = this->left_operand()->value();
		std::unique_ptr<class value> right_v = this->right_operand()->value();
		boolean_value* left_boolean_v = dynamic_cast<boolean_value*>(left_v.get());
		boolean_value* right_boolean_v = dynamic_cast<boolean_value*>(right_v.get());

		if (!left_boolean_v || !right_boolean_v)
			throw std::invalid_argument("calc::logical_or_expr::value");

		return std::make_unique<boolean_value>(left_boolean_v->to_bool()
		                                       || right_boolean_v->to_bool());
	}

	boolean::boolean(bool v) noexcept : _value(v) {}

	std::unique_ptr<class value> boolean::value() const {
		return std::make_unique<boolean_value>(this->_value);
	}

	integer::integer(std::int32_t v) noexcept : _value(v) {}

	std::unique_ptr<class value> integer::value() const {
		return std::make_unique<integer_value>(this->_value);
	}

	// -----------------------------------------------------------------------
	// Types
	// -----------------------------------------------------------------------

	type::type(const char* name) : _name(name) {}

	type::type(const std::string& name) : _name(name) {}

	type::~type() {}

	std::string type::name() const {
		return this->_name;
	}

	bool operator==(const type& type1, const type& type2) {
		return &type1 == &type2;
	}

	bool operator!=(const type& type1, const type& type2) {
		return &type1 != &type2;
	}

	const boolean_type boolean_type::instance;
	const integer_type integer_type::instance;

	boolean_type::boolean_type() : type("boolean") {}

	integer_type::integer_type() : type("integer") {}

	// -----------------------------------------------------------------------
	// Values
	// -----------------------------------------------------------------------

	value::value(const class type& type) noexcept : _type(type) {}

	value::value(const value& other) noexcept : _type(other._type) {}

	value::~value() {}

	const class type& value::type() const noexcept {
		return this->_type;
	}

	bool value::is_equal(const value& other) const noexcept {
		return this->do_is_equal(other);
	}

	bool operator==(const value& value1, const value& value2) {
		return &value1 == &value2 || value1.is_equal(value2);
	}

	bool operator!=(const value& value1, const value& value2) {
		return &value1 != &value2 && !value1.is_equal(value2);
	}

	boolean_value::boolean_value(bool v) noexcept :
		value(boolean_type::instance), _data(v)
	{}

	boolean_value::boolean_value(const boolean_value& other) noexcept :
		value(other), _data(other._data)
	{}

	boolean_value& boolean_value::operator=(bool v) noexcept {
		this->_data = v;
		return *this;
	}

	boolean_value& boolean_value::operator=(const boolean_value& other) noexcept {
		this->_data = other._data;
		return *this;
	}

	bool boolean_value::to_bool() const noexcept {
		return this->_data;
	}

	boolean_value::operator bool() const noexcept {
		return this->_data;
	}

	bool boolean_value::do_is_equal(const value& other) const noexcept {
		if (typeid(other) != typeid(boolean_value))
			return false;
		return this->to_bool() == dynamic_cast<const boolean_value&>(other).to_bool();
	}

	integer_value::integer_value(std::int32_t v) noexcept :
		value(integer_type::instance), _data(v)
	{}

	integer_value::integer_value(const integer_value& other) noexcept :
		value(other), _data(other._data)
	{}

	integer_value& integer_value::operator=(std::int32_t v) noexcept {
		this->_data = v;
		return *this;
	}

	integer_value& integer_value::operator=(const integer_value& other) noexcept {
		this->_data = other._data;
		return *this;
	}

	#define DEFINE_INTEGER_COMPOUND_ASSIGNMENT_OPERATOR(OP) \
		integer_value& integer_value::operator OP##=(std::int32_t v) noexcept { \
			*this = std::int32_t(*this) OP v; \
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

	integer_value& integer_value::operator++() noexcept {
		++(this->_data);
		return *this;
	}

	integer_value integer_value::operator++(int) noexcept {
		integer_value result(*this);
		++(*this);
		return result;
	}

	integer_value& integer_value::operator--() noexcept {
		--(this->_data);
		return *this;
	}

	integer_value integer_value::operator--(int) noexcept {
		integer_value result(*this);
		--(*this);
		return result;
	}

	std::int32_t integer_value::to_int32() const noexcept {
		return this->_data;
	}

	integer_value::operator std::int32_t() const noexcept {
		return this->_data;
	}

	bool integer_value::do_is_equal(const value& other) const noexcept {
		if (typeid(other) != typeid(integer_value))
			return false;
		return this->to_int32() == dynamic_cast<const integer_value&>(other).to_int32();
	}
} // namespace calc
