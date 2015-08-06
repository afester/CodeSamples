
#ifndef __EXPRESSION_AST_H__
#define __EXPRESSION_AST_H__

#include <string>
#include <memory>

#include "SymbolTable.h"


class Node {
public:
	Node();

	~Node();

	virtual double evaluate() = 0;

	virtual void dump() = 0;
};


typedef enum {OP_ADD, OP_SUB, OP_MUL, OP_DIV} BINARY_OP;


class BinaryOperationNode : public Node {
	BINARY_OP oper;
	std::auto_ptr<Node> left;
	std::auto_ptr<Node> right;

public:
	BinaryOperationNode(BINARY_OP oper, Node* left, Node* right);

	virtual double evaluate();

	virtual void dump();
};


class NumberNode : public Node {
	double value;

public:
	NumberNode(double value);

	virtual double evaluate();

	virtual void dump();
};


class IdNode : public Node {
	const SymbolTableEntry& entry;

public:
	IdNode(const SymbolTableEntry& stEntry);

	virtual double evaluate();

	virtual void dump();
};


class FunctionNode : public Node {
	MATH_FUN theFunction;
	std::auto_ptr<Node> param;

public:
	FunctionNode(MATH_FUN theFunction, Node* param);

	virtual double evaluate();

	virtual void dump();
};

#endif
