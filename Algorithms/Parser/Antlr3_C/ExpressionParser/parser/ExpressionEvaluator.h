/*
 * ExpressionEvaluator.cpp
 *
 *  Created on: 31.08.2015
 *      Author: afester
 */

#include <string>
#include "SymbolTable.h"	// TODO: Remove dependency for cleaner DLL exports
#include "ExpressionAST.h"

#ifndef API_EXPORT
#define API_EXPORT
#endif

class API_EXPORT ExpressionEvaluator {
    SymbolTable st;
    std::string expr;

public:

	ExpressionEvaluator(const std::string& anExpr);

	void setVariable(const std::string& name, double value);

	double evaluateExpression();
};


class API_EXPORT ExpressionEvaluator2 {
    SymbolTable st;
    std::string expr;
    Node* exprAst;

public:

	ExpressionEvaluator2(const std::string& anExpr);

	void setVariable(const std::string& name, double value);

	void parse();

	void dump();

	double evaluateExpression();
};
