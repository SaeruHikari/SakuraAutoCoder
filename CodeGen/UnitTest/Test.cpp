#include <iostream>
#include "CodeGen/refl.rule.hxx"

struct WTF
{

};

int main(void)
{
	std::cout << Sakura::refl::GetTypeId<int>() << std::endl;
	std::cout << Sakura::refl::SClass<int>().GetName() << std::endl;
	std::cout << Sakura::refl::SClass<void>().GetName() << std::endl;
	int testVal = 15;
	int byPass = Sakura::refl::SClass<int>().GetField(testVal, "").GetT<int>();
	// => int byPass = Sakura::refl::dyn::GetClass("int").GetField(testVal, "").GetT("int");
	bool tcheck = Sakura::refl::SClass<int>().GetField(testVal, "").IsT<int>();
	return 0;
}