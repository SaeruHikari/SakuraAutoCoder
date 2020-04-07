#include <iostream>
#include "include/refl.rule.hxx"

using namespace Sakura::refl;

struct [[component]] TestComponent
{
	[[meta("SaeruHikari")]] float attrib = 123.f;
	std::string name = "TestComp";
	inline static const constexpr float staticAttrib = 155;
	void Method() { std::cout << "TestMethod!"; };
};

struct [[component]] TestComponentWrap
{
	TestComponent comp;
	inline static const TestComponent statComp = { 14221.f, "Stat" };
	float wtf = 155.f;
};

template<>
struct ClassInfo<TestComponent>
{
	inline static constexpr const char* GetClassName()
	{
		return "TestComponent";
	}
	inline static const constexpr Meta::MetaPiece attrib_meta[1] =
	{
		{"meta", "SaeruHikari"}
	};
	inline static constexpr const auto attrib_tup = hana::make_tuple(
		Field{
			"attrib", "float", offsetof(TestComponent, attrib),
			{attrib_meta}
		}, &TestComponent::attrib);
	inline static constexpr const auto name_tup = hana::make_tuple(
		Field{
			"name", "string", offsetof(TestComponent, name),
			{nullptr}
		}, &TestComponent::name);
	inline static constexpr auto all_tup = hana::make_tuple(attrib_tup, name_tup);

	inline static constexpr auto staticAttrib_tup = hana::make_tuple(Field{
			"staticAttrib", "float", 0, {nullptr}
		}, &TestComponent::name);
	inline static constexpr auto all_static_tup = hana::make_tuple(staticAttrib_tup);

	inline static constexpr auto Method_tup = hana::make_tuple(Field{
			"Method", "void(TestComponent::*)()", 0, {nullptr}
		}, &TestComponent::Method);
	inline static constexpr auto all_method_tup = hana::make_tuple(Method_tup);

	// Dynamic data
	inline static const constexpr Field fields[2] =
	{
		{
			"attrib", "float", offsetof(TestComponent, attrib),
				{attrib_meta}
		},
		{
			"name", "string", offsetof(TestComponent, name), {nullptr}
		}
	};
};

template<>
struct ClassInfo<TestComponentWrap>
{
	inline static constexpr const char* GetClassName()
	{
		return "TestComponentWrap";
	}
	inline static constexpr const auto wtf_tup = hana::make_tuple(Field{
			"wtf", "float", offsetof(TestComponentWrap, wtf), {nullptr}
		}, &TestComponentWrap::wtf);
	inline static constexpr const auto comp_tup = hana::make_tuple(Field{
			"comp", "TestComponent", offsetof(TestComponentWrap, comp), {nullptr}
		}, &TestComponentWrap::comp);
	inline static constexpr auto all_tup = hana::make_tuple(comp_tup, wtf_tup);
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
			auto field = SClass<T>::info::fields[i];
			auto fieldfield = SClass<Field>::info::fields[j];
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
	TestComponent testComp;
	TestComponentWrap testCompWrap;
	SClass<std::decay<decltype(testComp)>::type>::ForEachField(testComp,
		Sakura::overload(
			[](const std::string& field, const Field& meta) {
				std::cout << "string: " << meta.name << ": " << field << std::endl;
			},
			[](auto&& field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			}));
	std::cout << std::endl << std::endl;
	SClass<std::decay<decltype(testCompWrap)>::type>::ForEachFieldAtomic(testCompWrap,
		Sakura::overload(
			[](float field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			},
			[](const std::string& field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			}));

	SClass<std::decay<decltype(testComp)>::type>::ForEachMethod(testComp,
		[&](auto&& method, const Field& meta)
		{
			if constexpr
				(std::is_same<std::decay<decltype(method)>::type, void(TestComponent::*)()>())
			{
				std::cout << meta.name << ": " << meta.type << std::endl;
				(testComp.*method)();
			}
		});
	return 0;
}
