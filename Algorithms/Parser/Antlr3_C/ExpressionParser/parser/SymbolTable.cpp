#include <iostream>
#include <sstream>
#include <math.h>
#include "SymbolTable.h"

SymbolTableEntry::SymbolTableEntry() {
}


SymbolTableEntry::SymbolTableEntry(const std::string& name) :
			id(name), kind(SYM_VAR), val(0) {
}


SymbolTableEntry::SymbolTableEntry(const std::string& name, MATH_FUN aFun) :
			id(name), kind(SYM_FUN), fun(aFun) {
}


SymbolTableEntry::SymbolTableEntry(const std::string& name, double value) :
			id(name), kind(SYM_CONST), val(value) {
}


SymbolTableEntry::~SymbolTableEntry() {
}

std::string SymbolTableEntry::kindToName(SYMBOL_KIND kind) const {

	switch(kind) {
		case SYM_VAR : return "VAR";

		case SYM_CONST : return "CONST";

		case SYM_FUN : return "FUN";

		default : break;
	}

	return "UNKNOWN";
}


std::string SymbolTableEntry::toString() const {
	std::stringstream result;

	result << kindToName(kind) << " " << id;
	if (kind != SYM_FUN) {
		result << ": " << val;
	}
	return result.str();
}


SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::init() {
	// std::cerr << "Initializing Symbol table ..." << std::endl;

	table["x"] = SymbolTableEntry("x");

	table["e"] = SymbolTableEntry("e", M_E);
	table["pi"] = SymbolTableEntry("pi", M_PI);

    table["sin"] = SymbolTableEntry("sin", sin);
    table["cos"] = SymbolTableEntry("cos", cos);
}

void SymbolTable::dump() {
	std::cerr << "Dump of Symbol Table" << std::endl;
	std::cerr << "--------------------------" << std::endl;

	std::map<std::string, SymbolTableEntry>::const_iterator it = table.begin();
	while(it != table.end()) {
		std::cerr << ": " << it->second.toString() << std::endl;
		it++;
	}

	std::cerr << "--------------------------" << std::endl;
}

void SymbolTable::setValue(const std::string& id, double value) {
	std::map<std::string, SymbolTableEntry>::iterator it = table.find(id);
	if (it == table.end()) {
		std::cerr << "ERROR: Unknown symbol " << id << std::endl;
		return;
	}

	SymbolTableEntry& entry = it->second;
	if (entry.kind == SYM_CONST) {
		std::cerr << "ERROR: Can not set new value for constant " << id << std::endl;
		return;
	}
	if (entry.kind == SYM_FUN) {
		std::cerr << "ERROR: Can not set new value for function " << id << std::endl;
		return;
	}

	entry.val = value;
}

double SymbolTable::getValue(const std::string& id) const {
	std::map<std::string, SymbolTableEntry>::const_iterator it = table.find(id);
	if (it == table.end()) {
		std::cerr << "ERROR: Unknown symbol " << id << std::endl;
		return 0;
	}

	const SymbolTableEntry& entry = it->second;
	if (entry.kind == SYM_FUN) {
		std::cerr << "ERROR: Function \"" << id << "\" requires an argument." << std::endl;
		return 0;
	}

	return entry.val;
}


double SymbolTable::executeFunction(const std::string& id, double argument) const {
	std::map<std::string, SymbolTableEntry>::const_iterator it = table.find(id);
	if (it == table.end()) {
		std::cerr << "ERROR: Unknown function " << id << std::endl;
		return 0;
	}

	const SymbolTableEntry& entry = it->second;
	if (entry.kind != SYM_FUN) {
		std::cerr << "ERROR: \"" << id << "\" is not a function." << std::endl;
		return 0;
	}

	return entry.fun(argument);
}


const SymbolTableEntry& SymbolTable::getSymbol(const std::string& id) const {
	std::map<std::string, SymbolTableEntry>::const_iterator it = table.find(id);
	if (it == table.end()) {
		std::cerr << "ERROR: Unknown symbol " << id << std::endl;
		return SymbolTableEntry();	// TODO!!!
	}

	return it->second;
}
