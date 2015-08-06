#include <iostream>

#include "ExpressionAST.h"


static int dumpIndent = 0;  // TODO

static std::string prefix() {
	std::string indent = "                                              ";
	return indent.substr(0, 2*dumpIndent);
}

Node::Node() {
}

Node::~Node() {
}


BinaryOperationNode::BinaryOperationNode(BINARY_OP anOper, Node* leftNode, Node* rightNode) :
	oper(anOper), left(leftNode), right(rightNode) {
}

double BinaryOperationNode::evaluate() {
	switch(oper) {
		case OP_MUL :
			return left->evaluate() * right->evaluate();

		case OP_DIV :
			return left->evaluate() / right->evaluate();

		case OP_ADD :
			return left->evaluate() + right->evaluate();

		case OP_SUB :
			return left->evaluate() - right->evaluate();
	}

	return 0;
}

void BinaryOperationNode::dump() {
	switch(oper) {
		case OP_MUL :
			std::cerr << prefix() << "*" << std::endl;
			break;

		case OP_DIV :
			std::cerr << prefix() << "/" << std::endl;
			break;

		case OP_ADD :
			std::cerr << prefix() << "+" << std::endl;
			break;

		case OP_SUB :
			std::cerr << prefix() << "-" << std::endl;
			break;
	}

	dumpIndent++;
	left->dump();
	right->dump();
	dumpIndent--;
}


NumberNode::NumberNode(double aValue) : value(aValue) {
}

double NumberNode::evaluate() {
	return value;
}


void NumberNode::dump() {
	std::cerr << prefix() << "Number:" << value << std::endl;
}


IdNode::IdNode(const SymbolTableEntry& stEntry) : entry(stEntry) {
}

double IdNode::evaluate() {
	return entry.getValue();
}

void IdNode::dump() {
	std::cerr << prefix() << "Id:" << entry.getName() << std::endl;
}


FunctionNode::FunctionNode(MATH_FUN aFunction, Node* aParam) :
	theFunction(aFunction), param(aParam) {
}

double FunctionNode::evaluate() {
	return theFunction(param->evaluate());
}

void FunctionNode::dump() {
	std::cerr << prefix() << "Func:" << theFunction << std::endl;
	dumpIndent++;
	param->dump();
	dumpIndent--;
}
