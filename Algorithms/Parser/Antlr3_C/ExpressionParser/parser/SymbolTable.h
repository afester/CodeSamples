
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <map>

#ifndef API_EXPORT
#define API_EXPORT
#endif

typedef double (*MATH_FUN)(double arg);
typedef enum {SYM_VAR, SYM_CONST, SYM_FUN} SYMBOL_KIND;

class  API_EXPORT SymbolTableEntry {
	std::string id;
	SYMBOL_KIND kind;

	union {
		MATH_FUN fun;
		double val;
	};

	friend class SymbolTable;

public:
	SymbolTableEntry();

	// variable
	SymbolTableEntry(const std::string& id);

	// constant
	SymbolTableEntry(const std::string& id, double value);

	// f(x)
	SymbolTableEntry(const std::string& id, MATH_FUN fun);

	virtual ~SymbolTableEntry();

	inline std::string getName() const { return id; }

	inline SYMBOL_KIND getKind() const { return kind; }

	inline MATH_FUN getFun() const { return fun; }

	inline double getValue() const { return val; }

	std::string kindToName(SYMBOL_KIND kind) const;

	std::string toString() const;
};



class API_EXPORT SymbolTable {
	std::map<std::string, SymbolTableEntry> table;

public:
	SymbolTable();

	virtual ~SymbolTable();

	void init();

	void dump();

	void setValue(const std::string& id, double value);

	double getValue(const std::string& id) const;

	double executeFunction(const std::string& id, double argument) const;

	const SymbolTableEntry& getSymbol(const std::string& name) const;
};

#endif
