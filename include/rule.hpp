#pragma once

#include <string>
#include <vector>

using Value = std::variant<int, double, std::string, bool>;

enum class Op
{
	Equals,
	NotEquals,
	GreaterThan,
	LessThan,
	And,
	Or,
	Not
};

struct Expression;
using ExpressionPtr = std::shared_ptr<Expression>;

struct Expression {
	Op op;
	std::string variableName;
	Value value;
	ExpressionPtr left, right;
	std::vector<ExpressionPtr> children;
};

struct Rule {
	int id;
	std::string name;
	ExpressionPtr condition;
	std::string action;
};