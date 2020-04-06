#include <iostream>
#include "CodeGen/refl.rule.hxx"

using namespace Sakura::refl;

struct [[component]] TestComponent
{
	[[meta("Fxxk ISO C++23")]] float attrib;
	std::string name;
	[[meta("Can't wait to use compile-time-reflection")]]
	void Method(std::string inVal, TestComponent& inRef) {};
	static float staticAttrib;
};

struct TestComponentWrap
{
	TestComponent comp;
	float wtf;
};

template<>
struct ClassInfo<TestComponent>
{
	inline static constexpr const char* GetClassName()
	{
		return "TestComponent";
	}
	inline static const constexpr Meta::MetaPiece attrib_meta[2] = 
	{
		{"meta", "Fxxk ISO C++23"},
		{"meta", "Fxxk ISO C++23"}
	};
	inline static const constexpr Field fields[2] =
	{
		//
		{
			"attrib", "float", EClass, offsetof(TestComponent, attrib), sizeof(float), 
				{attrib_meta, Sakura::refl::arraySize(attrib_meta)}
		},
		{
			"name", "string", EClass, offsetof(TestComponent, name), sizeof(std::string),
				{nullptr, 0}
		}
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



void AtomicStream(const Reference& ref, const std::string& type)
{
	if (type.starts_with("uint64"))
		std::cout << ref.GetT<uint64_t>();
	else if (type.starts_with("uint32"))
		std::cout << ref.GetT<uint32_t>();
	else if (type.starts_with("uint8"))
		std::cout << ref.GetT<uint8_t>();
	else if (type.starts_with("uint16"))
		std::cout << ref.GetT<uint16_t>();
	else if (type.starts_with("string"))
		std::cout << ref.GetT<std::string>();
	else if (type.starts_with("cstr"))
		std::cout << ref.GetT<const char*>();
	else if (type.starts_with("meta"))
	{
		for (auto i = 0u; i < ref.GetT<Sakura::refl::Meta>().metaCount; i++)
		{
			if (ref.GetT<Sakura::refl::Meta>().metas[i].title != nullptr)
				std::cout << "\n    " << ref.GetT<Sakura::refl::Meta>().metas[i].title << ": ";
			if (ref.GetT<Sakura::refl::Meta>().metas[i].value != nullptr)
				std::cout << ref.GetT<Sakura::refl::Meta>().metas[i].value;
		}
	}
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
		std::cout << std::endl;
	}
}

int main(void)
{
	printFieldMeta<TestComponent>();
	return 0;
}