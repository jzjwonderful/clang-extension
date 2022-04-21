#include <clang-c/Index.h>

#include <functional>
#include <iostream>
#include <list>
#include <string>

using namespace std;

class MyStruc {
  /**
   * @brief name
   */
  string name;
  /**
   * @brief gender
   */
  string gender;
};

ostream &operator<<(ostream &stream, const CXString &str) {
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

int main() {
  CXIndex index = clang_createIndex(0, 0);
  //  CXTranslationUnit unit = clang_parseTranslationUnit(
  //      index, "..\\..\\main.cpp", nullptr, 0,
  //      nullptr, 0, CXTranslationUnit_None);

  CXTranslationUnit unit;
  auto ret = clang_parseTranslationUnit2(
      index, "..\\..\\main.cpp", nullptr, 0, nullptr, 0,
      CXTranslationUnit_SingleFileParse, &unit);
  if (unit == nullptr || ret != CXError_Success) {
    cerr << "Unable to parse translation unit. Quitting." << endl;
    exit(-1);
  }

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      [](CXCursor c, CXCursor parent, CXClientData client_data) {
        //

        if (clang_getCursorKind(c) == CXCursorKind::CXCursor_ClassDecl) {
          CXSourceRange sr = clang_getCursorExtent(c);
          unsigned int line = 0, column = 0, offset = 0;
          CXFile cf;
          clang_getSpellingLocation(clang_getRangeStart(sr), &cf, &line,
                                    &column, &offset);
          cout << "start to line:" << line << ",column:" << column
               << ",offset:" << offset << endl;

          clang_getSpellingLocation(clang_getRangeEnd(sr), &cf, &line, &column,
                                    &offset);
          cout << "end to line:" << line << ",column:" << column
               << ",offset:" << offset << endl;

          CXCursorSet set;
          clang_visitChildren(
              c,
              [](CXCursor c, CXCursor parent, CXClientData client_data) {
                auto set = static_cast<CXCursorSet *>(client_data);
                clang_CXCursorSet_insert(*set, c);
                return CXChildVisit_Continue;
              },
              &set);

          return CXChildVisit_Continue;
        }

        //        cout << "Cursor '" << clang_getCursorDisplayName(c) << "' of
        //        kind '"
        //             << clang_getCursorKindSpelling(clang_getCursorKind(c));

        //        auto location = clang_getCursorLocation(c);
        //        CXFile cf;
        //        unsigned int line = 0, column = 0, offset = 0;
        //        clang_getSpellingLocation(location, &cf, &line, &column,
        //        &offset); cout << " " << clang_getFileName(cf) << " "
        //             << "line:" << line << ",column:" << column << ",offset:"
        //             << offset;
        //        cout << endl;

        return CXChildVisit_Recurse;
      },
      nullptr);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
  return 0;
}
