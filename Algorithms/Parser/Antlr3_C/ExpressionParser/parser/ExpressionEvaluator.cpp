/*
 * ExpressionEvaluator.cpp
 *
 *  Created on: 31.08.2015
 *      Author: afester
 */

#define API_EXPORT __declspec(dllexport)
#include "ExpressionEvaluator.h"

#include "ExpressionParser.hpp"
#include "SymbolTable.h"
#include "ExpressionASTParser.hpp"
#include "ExpressionAST.h"


ExpressionEvaluator::ExpressionEvaluator(const std::string& anExpr) : expr(anExpr) {
	st.init();
}

void ExpressionEvaluator::setVariable(const std::string& name, double value) {
	st.setValue(name, value);
}

double ExpressionEvaluator::evaluateExpression() {
	//std::cerr << "Expression:" << expr << std::endl;
	//st.dump();

	// TODO: There should be no need to recreate a new lexer and parser,
	// however the reset() methods of lexer and parser do not work (yet)
	// a possible solution could be to create an AST in the parser,
	// and then reuse the AST during evaluation
	ExpressionLexer::InputStreamType
			input((const ANTLR_UINT8*)expr.c_str(), ANTLR_ENC_8BIT,
					expr.length(), NULL);
	ExpressionLexer lxr(&input);

	ExpressionParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource() );
	ExpressionParser psr(&tstream);
	psr.setSymbolTable(&st);

	return psr.expr();
}


ExpressionEvaluator2::ExpressionEvaluator2(const std::string& anExpr) : expr(anExpr), exprAst(0) {
	st.init();
}

void ExpressionEvaluator2::setVariable(const std::string& name, double value) {
	st.setValue(name, value);
}


void ExpressionEvaluator2::parse() {
	ExpressionASTLexer::InputStreamType
			input((const ANTLR_UINT8*)expr.c_str(), ANTLR_ENC_8BIT,
					expr.length(), NULL);
	ExpressionASTLexer lxr(&input);

	ExpressionASTParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource() );
	ExpressionASTParser psr(&tstream);
	psr.setSymbolTable(&st);

	exprAst = psr.expr();
}


void ExpressionEvaluator2::dump() {
	exprAst->dump();
}

double ExpressionEvaluator2::evaluateExpression() {
	return exprAst->evaluate();
}
