#include <iostream>
#include "CodeGen/refl.rule.hxx"

using namespace Sakura::refl;

struct [[component]] TestComponent
{
	[[norefl]] float attrib;
	[[meta("Fxxk ISO C++23")]] std::string name;
	[[meta("Can't wait to use compile-time-reflection")]]
	void Method(std::string inVal, TestComponent& inRef) {};
	static float staticAttrib;
};

template<> 
inline static constexpr const char* Sakura::refl::GetClassNameT<TestComponent>()
{ return "TestComponent"; }

template<>
inline static const Reference Sakura::refl::GetFieldT<TestComponent>(const Reference& o, const std::string&)
{
	return o;
}


int main(void)
{
	std::cout << Sakura::refl::GetTypeId<int>() << std::endl;
	std::cout << Sakura::refl::GetTypeId<TestComponent>() << std::endl;
	std::cout << SClass<TestComponent>::GetName() << std::endl;
	std::cout << Sakura::refl::SClass<int>::GetName() << std::endl;
	std::cout << Sakura::refl::SClass<const int>::GetName() << std::endl;

	int testVal = 15;
	const int& targVal = testVal;
	Sakura::refl::SClass<int>::GetField(testVal, "");
	// => int byPass = Sakura::refl::dyn::GetClass("int").GetField(testVal, "").GetT("int");
	/*bool tcheck = Sakura::refl::SClass<int>().GetField(targVal, "").IsT<int>();
	bool ccheck = Sakura::refl::SClass<int>().GetField(targVal, "").IsT<int>();*/
	return 0;
}