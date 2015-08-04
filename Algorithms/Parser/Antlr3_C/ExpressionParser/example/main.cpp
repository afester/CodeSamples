#include <iostream>
#include "ExpressionParser.hpp"
#include "SymbolTable.h"
#include "Math.h"


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

	void test() {
	    ExpressionLexer::InputStreamType
	    		input((const ANTLR_UINT8*)expr.c_str(), ANTLR_ENC_8BIT,
	    				expr.length(), NULL);

	    ExpressionLexer lxr(&input);

	    ExpressionParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource() );
	    ExpressionParser psr(&tstream);
	    psr.setSymbolTable(&st);

	    for (double x = 0;  x < 4;  x += 0.5) {
			setVariable("x", x);
			lxr.reset();
			input.reset();

		    ExpressionLexer::InputStreamType
		    		input((const ANTLR_UINT8*)expr.c_str(), ANTLR_ENC_8BIT,
		    				expr.length(), NULL);
		    ExpressionParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource() );
			lxr.setCharStream(&input);
			psr.init(&tstream);
			psr.reset();
			double result = psr.expr();
			std::cerr << "RES:" << result << std::endl;
	    }
	}
};



void testFunction() {
    ExpressionEvaluator ev("sin(x)");
    ev.test();
    return;

    for (double x = 0;  x < 2*M_PI;  x += 0.1) {
    	ev.setVariable("x", x);
    	double result = ev.evaluateExpression();
    	std::cerr << "Result: " << result << std::endl;
    }
}


int main(int argc, char *argv[]) {
    std::cout << "ExpressionParser example" << std::endl;
    testFunction();

/*
    ExpressionEvaluator ev("pi / x");
    for (int i = 0;  i < 10;  i++) {
    	ev.setVariable("x", i);
    	double result = ev.evaluateExpression();
    	std::cerr << "Result: " << result << std::endl;
    }
*/

    return 0;
}
