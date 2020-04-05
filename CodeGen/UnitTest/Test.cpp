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
struct SClass<TestComponent> final : public IType
{
	inline static const constexpr char name[] = "TestComponent";
	inline static const constexpr std::size_t id_ =
		_Fnv1a_append_bytes(Sakura::refl::_FNV_offset_basis, name, length(name) * sizeof(char));

	virtual const char* GetName() const override final;
};

const char* SClass<TestComponent>::GetName() const
{
	return name;
}

int main(void)
{
	std::cout << Sakura::refl::GetTypeId<int>() << std::endl;
	std::cout << Sakura::refl::GetTypeId<TestComponent>() << std::endl;
	std::cout << SClass<TestComponent>().GetName() << std::endl;
	std::cout << Sakura::refl::SClass<int>().GetName() << std::endl;
	std::cout << Sakura::refl::SClass<void>().GetName() << std::endl;
	int testVal = 15;
	const int& targVal = testVal;
	int byPass = Sakura::refl::SClass<int>().GetField(testVal, "").GetT<int>();
	// => int byPass = Sakura::refl::dyn::GetClass("int").GetField(testVal, "").GetT("int");
	bool tcheck = Sakura::refl::SClass<int>().GetField(targVal, "").IsT<int>();
	bool ccheck = Sakura::refl::SClass<int>().GetField(targVal, "").IsT<int>();
	return 0;
}