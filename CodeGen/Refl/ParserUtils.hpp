#pragma once

#include <string>
#include <clang-c/Index.h>

namespace Sakura::AutoCoder
{
	namespace Parser
	{
		std::string Convert(const CXString& s);

		std::string GetFullName(CXCursor cursor);
		std::string GetName(const CXType& type);
		std::string GetFile(const CXCursor& cursor);

		bool IsRecursivelyPublic(CXCursor cursor);
	}
}