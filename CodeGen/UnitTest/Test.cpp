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
struct ClassInfo<TestComponent>
{
	inline static constexpr const char* GetClassNameT()
	{
		return "TestComponent";
	}
	inline static const constexpr Field fields[2] =
	{
		{"attrib", "float", EClass, offsetof(TestComponent, attrib)},
		{"name", "string", EClass, offsetof(TestComponent, name)}
	};
};

template<>
inline static const Reference Sakura::refl::GetFieldT<TestComponent>(
	const Reference& o, const std::string& name)
{
	auto ref = o.GetT<TestComponent>();
	if (name == "attrib")
		return ref.attrib;
	else if (name == "name")
		return ref.name;
	assert(0 && "No field of this name.");
	return o;
}



template<typename T>
void printFieldMeta()
{
	for (auto i = 0; i < SClass<T>::GetFieldCount(); i++)
	{
		for (auto j = 0; j < SClass<Field>::GetFieldCount(); j++)
		{
			auto field = SClass<T>::ClassInfo::fields[i];
			auto fieldfield = SClass<Field>::ClassInfo::fields[j];
			std::cout << fieldfield.name << ": ";
			AtomicStream(SClass<Field>::GetField(field, fieldfield.name), fieldfield.type);
			std::cout << std::endl;
		}
	}
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
	bool tcheck = Sakura::refl::SClass<int>::GetField(targVal, "").IsT<int>();
	std::cout << Sakura::refl::SClass<TestComponent>::GetFieldCount() << std::endl;
	uint64_t tval = 15;
	AtomicStream(tval, "uint64_t");
	std::cout << std::endl;
	printFieldMeta<TestComponent>();
	return 0;
}