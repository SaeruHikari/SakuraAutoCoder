#include "Parser.hpp"
#include "ParserUtils.hpp"

#include <iostream>

#include <clang-c/Index.h>

using namespace Sakura::AutoCoder;
using namespace std;

namespace
{
	ostream& operator<<(ostream& s, const CXString& str)
	{
		s << Parser::Convert(str);
		return s;
	}


}  // namespace