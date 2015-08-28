#include <iostream>
#define _USE_MATH_DEFINES // Required by MSVC
#include <math.h>

#include "ExpressionParser.hpp"
#include "SymbolTable.h"
#include "Profile.h"

#include "ExpressionASTParser.hpp"
#include "ExpressionAST.h"


class ExpressionEvaluator {
    SymbolTable st;
    std::string expr;

public:

	ExpressionEvaluator(const std::string& anExpr) : expr(anExpr) {
	    st.init();
	}


	inline void setVariable(const std::string& name, double value) {
		st.setValue(name, value);
	}

	double evaluateExpression() {
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

};




class ExpressionEvaluator2 {
    SymbolTable st;
    std::string expr;
    Node* exprAst;

public:

	ExpressionEvaluator2(const std::string& anExpr) : expr(anExpr), exprAst(0) {
	    st.init();
	}


	inline void setVariable(const std::string& name, double value) {
		st.setValue(name, value);
	}

	void parse() {
	    ExpressionASTLexer::InputStreamType
	    		input((const ANTLR_UINT8*)expr.c_str(), ANTLR_ENC_8BIT,
	    				expr.length(), NULL);
	    ExpressionASTLexer lxr(&input);

	    ExpressionASTParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource() );
	    ExpressionASTParser psr(&tstream);
	    psr.setSymbolTable(&st);

		exprAst = psr.expr();
	}


	void dump() {
		exprAst->dump();
	}

	double evaluateExpression() {
		return exprAst->evaluate();
	}
};



void testFunction() {
    ExpressionEvaluator ev("sin(x)");
    for (double x = 0;  x < 2*M_PI;  x += 0.1) {
    	ev.setVariable("x", x);
    	double result = ev.evaluateExpression();
    	std::cerr << "Result: " << result << std::endl;
    }
}


void testPerformance()  {
    ProcessTimeCounter p1;
    RealTimeCounter r1;

    p1.start();
    r1.start();

    ExpressionEvaluator ev("(3 + cos(x) * sin(x)) / 5");
    for (int run = 0;  run < 10000;  run++) {
       for (double x = 0; x < 2;  x += 0.1) {
    	   ev.evaluateExpression();
       }
    }

    p1.stop();
    r1.stop();

    std::cerr << p1 << std::endl;
    std::cerr << r1 << std::endl;
}


void testPerformance2()  {
    ProcessTimeCounter p1;
    RealTimeCounter r1;

    p1.start();
    r1.start();

    ExpressionEvaluator2 ev("(3 + cos(x) * sin(x)) / 5");
    ev.parse();

    for (int run = 0;  run < 10000;  run++) {
       for (double x = 0; x < 2;  x += 0.1) {
    	   ev.evaluateExpression();
       }
    }

    p1.stop();
    r1.stop();

    std::cerr << p1 << std::endl;
    std::cerr << r1 << std::endl;
}

void testAST1() {
    // ExpressionEvaluator2 ev("sin(x)");
    //ExpressionEvaluator2 ev("(3 + cos(x) * sin(x)) / 5");
	ExpressionEvaluator ev("(3 + cos(x) * sin(x)) / 5");

    std::cerr << "Parsing ..." << std::endl;
    // ev.parse();

    std::cerr << "Evaluating ..." << std::endl;
    double result = ev.evaluateExpression();
    std::cerr << "Result: " << result << std::endl;

    //std::cerr << "Dumping ..." << std::endl;
    //ev.dump();
}


double evaluate(const std::string& expr) {
    ExpressionEvaluator ev(expr);
    return ev.evaluateExpression();
}

void testFloat()  {
	std::string expr = "0";
	double result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "5";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "-5";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "1045";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "-9000";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "1.005";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "-10.15";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "10";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "1.9 + 0.1";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;
}

double evaluate2(const std::string& expr) {
    ExpressionEvaluator2 ev(expr);
    return ev.evaluateExpression();
}

void testFloatAST()  {
	std::string expr = "0";
	double result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "5";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "-5";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "1045";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "-9000";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "1.005";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "-10.15";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "10";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;

	expr = "1.9 + 0.1";
	result = evaluate(expr);
	std::cerr << "\"" << expr << "\" => " << result << std::endl;
}


int main(int argc, char *argv[]) {
    std::cout << "ExpressionParser example" << std::endl;

    //testAST1();
    //testFunction();
    //testPerformance();
    //testPerformance2();
    testFloat();

    return 0;
}
