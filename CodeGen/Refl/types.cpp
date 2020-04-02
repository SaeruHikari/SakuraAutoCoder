#include "types.hpp"

using namespace Sakura::AutoCoder;

TypeBase::TypeBase(std::string _file, std::string _full_name)
	:full_name(std::move(_full_name)), file(std::move(_full_name))
{

}

TypeBase::~TypeBase() = default;

const std::string& TypeBase::GetFullName() const
{
	return full_name;
}

const std::string& TypeBase::GetName() const
{
	return full_name;
}

const std::string& TypeBase::GetFile() const
{
	return file;
}

const std::string& Sakura::AutoCoder::TypeBase::Transform(Type type)
{
	switch (type)
	{
	case Sakura::AutoCoder::TypeBase::Type::Enum:
		return Sakura::AutoCoder::TypeBase::enumStr;
	case Sakura::AutoCoder::TypeBase::Type::Function:
		return Sakura::AutoCoder::TypeBase::functionStr;
	case Sakura::AutoCoder::TypeBase::Type::Class:
		return Sakura::AutoCoder::TypeBase::classStr;
	default:
		break;
	}
	return "WrongType";
}
Enum::Enum(std::string file, std::string full_name)
	:TypeBase(std::move(file), std::move(full_name))
{

}

Enum::Type Enum::GetType() const
{
	return Type::Enum;
}

Function::Function(std::string file, std::string full_name)
	:TypeBase(std::move(file), std::move(full_name))
{

}

Function::Type Function::GetType() const
{
	return Type::Function;
}

Class::Class(std::string file, std::string full_name)
	:TypeBase(std::move(file), std::move(full_name))
{

}

Class::Type Class::GetType() const
{
	return Type::Class;
}