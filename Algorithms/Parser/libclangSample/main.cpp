
#include <iostream>
#include <string>
#include <clang-c/Index.h>

std::string kindName(CXCursorKind k) {
   return clang_getCString (clang_getCursorKindSpelling(k) );
}

static int level = 0;
std::string getIndent() {
    std::string space = "               ";
    return space.substr(0, level * 2);
}

CXChildVisitResult visitTypeRef(CXCursor c, CXCursor p, CXClientData d) {
   if (c.kind == CXCursor_TypeRef ) {
      CXType t = clang_getCursorType (c);
      CXString ts = clang_getTypeSpelling (t);
      std::cout << "   (TypeRef: " << clang_getCString(ts) << ")";
   }

   return CXChildVisit_Continue;
}

// visit structure nodes
CXChildVisitResult visitStruct(CXCursor c, CXCursor p, CXClientData d) {

   if (c.kind == CXCursor_FieldDecl) {

      CXType t = clang_getCursorType (c);
      CXString ts = clang_getTypeSpelling (t);

      CXString n = clang_getCursorSpelling(c);

       std::cout << getIndent() << kindName(c.kind) << " : " <<  clang_getCString(n) << "  // " << clang_getCString(ts);

      level++;
      clang_visitChildren(c, &visitTypeRef, NULL);
      level--;

      std::cout << std::endl;
   }
   return CXChildVisit_Continue;
}


// visit top level nodes
CXChildVisitResult visit(CXCursor c, CXCursor p, CXClientData d) {

   if (c.kind == CXCursor_StructDecl || c.kind == CXCursor_EnumDecl) {
      CXType t = clang_getCursorType (c);
      CXString ts = clang_getTypeSpelling (t);

      std::cout << getIndent() << kindName(c.kind) << ": " << clang_getCString(ts) << std::endl;

      level++;
      clang_visitChildren(c, &visitStruct, NULL);
      level--;
   }

   return CXChildVisit_Continue;
}


int main() {
  std::cout << "Parsing file ..." << std::endl;

  char const **args = new char const*[3];
  args[0] = "-x";
  args[1] = "c";
  args[2] = "sample.c";

  CXIndex idx = clang_createIndex(0, 1);
  CXTranslationUnit_Flags flags = CXTranslationUnit_DetailedPreprocessingRecord;
  CXTranslationUnit tu = 
    clang_parseTranslationUnit(idx, 0,
			       args,
			       3,
			       0,
			       0,
			       flags);

  std::cout << "Traversing AST ..." << std::endl;
  clang_visitChildren(clang_getTranslationUnitCursor(tu), &visit, NULL);

  clang_disposeTranslationUnit(tu);
  clang_disposeIndex(idx);

  return 0;
}
