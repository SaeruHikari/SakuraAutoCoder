#include "ParserUtils.hpp"

using namespace std;
using namespace Sakura::AutoCoder;

string Parser::Convert(const CXString& s)
{
	string result = clang_getCString(s);
	clang_disposeString(s);
	return result;
}

string Parser::GetFullName(CXCursor cursor)
{
	string name;
	while (clang_isDeclaration(clang_getCursorKind(cursor)) != 0)
	{
		string cur = Convert(clang_getCursorSpelling(cursor));
		if (name.empty())
			name = cur;
		else
			name = cur + "::" + name;
		cursor = clang_getCursorSemanticParent(cursor);
	}
	return name;
}

string Parser::GetName(const CXType& type)
{
	//TODO: unfortunately, this isn't good enough. It only works as long as the
	// type is fully qualified.
	return Convert(clang_getTypeSpelling(type));
}