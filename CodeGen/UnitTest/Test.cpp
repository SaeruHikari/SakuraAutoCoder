#include <iostream>
#include "include/refl.rule.hxx"
#include "component.refl.hxx"

using namespace Sakura::refl;
/*
template<>
inline const Reference Sakura::refl::GetFieldT<TestComponent>(
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
	else if (type.starts_with("cstr") || type.starts_with("const char*"))
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
	SClass<typename std::decay<T>::type>::ForEachFieldMeta(
			[](const Field& field)
			{
				SClass<Field>::ForEachField(field,
					[&](auto&& fieldfield, const Field& fieldmeta)
					{
						std::cout << fieldmeta.name << ": ";
						AtomicStream(SClass<Field>::GetField(
							field, fieldmeta.name), fieldmeta.type);
						std::cout << std::endl;
					});
			});
}*/

int main(void)
{
	//printFieldMeta<TestComponentWrap>();
	TestComponent testComp;

	TestComponentWrap testCompWrap;
	SClass<std::decay<decltype(testComp)>::type>::ForEachField(testComp,
		Sakura::overload(
			[](auto&& field, const Field& meta) {
				std::cout
					<< meta.type << " - "
					<< meta.name << ": " << field << std::endl;
			}));

	std::cout << std::endl << std::endl;
	SClass<std::decay<decltype(testCompWrap)>::type>::ForEachFieldAtomic(testCompWrap,
		Sakura::overload(
			[](float field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			},
			[](const std::string& field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			},
			[](TestComponent* field, const Field& meta) {
				std::cout << "TestCompPtr" << std::endl;
			}));

	std::cout << std::endl << std::endl;
	SClass<TestComponentWrap>::ForEachStaticFieldAtomic(
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
				(std::is_same<typename std::decay<decltype(method)>::type, void(TestComponent::*)()>())
			{
				std::cout << meta.name << ": " << meta.type << std::endl;
				(testComp.*method)();
			}
		});
	return 0;
}
