#include <iostream>
#include "include/refl.rule.hxx"

using namespace Sakura::refl;

struct [[component]][[descriptions("This is a test component")]] TestComponent
{
	[[meta("SaeruHikari")]] float attrib = 123.f;
	std::string name = "TestComp";
	inline static const constexpr float staticAttrib = 155;
	void Method() { std::cout << "TestMethod!"; };
};

struct [[component]] TestComponentWrap
{
	TestComponent comp;
	TestComponent* compPtr;
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
	inline static const constexpr Meta::MetaPiece meta[1] =
	{
		{"descriptions", "This is a test component."}
	};
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

	inline static constexpr auto Method_tup = hana::make_tuple(Field{
			"Method", "void(TestComponent::*)()", 0, {nullptr}
		}, &TestComponent::Method);


	inline static constexpr const auto all_field()
	{
		struct attrib_info
		{
			constexpr attrib_info() = default;
			Field fd = Field{
				"attrib", "float", offsetof(TestComponent, attrib), {attrib_meta} };
			decltype(&TestComponent::attrib) ptr = &TestComponent::attrib;
		};
		struct name_info
		{
			constexpr name_info() = default;
			Field fd = Field{
				"name", "cstr", offsetof(TestComponent, name), {nullptr} };
			decltype(&TestComponent::name) ptr = &TestComponent::name;
		};
		return hana::make_tuple(attrib_info(), name_info());
	}

	template<typename V, typename Fn>
	inline static constexpr const void for_each_field(V&& value, Fn&& fn)
	{
		detail::ForEachTuple(all_field(),
			[&value, &fn](auto&& field_schema)
			{
				fn(value.*(field_schema.ptr), field_schema.fd);
			});
	}

	inline static constexpr auto all_method_tup = hana::make_tuple(Method_tup);
	inline static constexpr auto all_static_tup = hana::make_tuple();
	inline static constexpr auto all_static_method_tup = hana::make_tuple();
};

#define SFIELD_INFO(NAME, T, MOTHER_BOARD, ...) \
	struct NAME##_info{\
		constexpr NAME##_info() = default;\
		Field fd = Field{\
			#NAME, #T, offsetof(MOTHER_BOARD, NAME), __VA_ARGS__ };\
		decltype(&MOTHER_BOARD::NAME) ptr = &MOTHER_BOARD::NAME;\
	};

template<>
struct ClassInfo<TestComponentWrap>
{
	inline static constexpr const char* GetClassName()
	{
		return "TestComponentWrap";
	}

	inline static const constexpr Meta::MetaPiece meta[1] =
	{
		{"descriptions", "This is a test component."}
	};

	inline static constexpr const auto all_field()
	{
		SFIELD_INFO(comp, TestComponent, TestComponentWrap, nullptr);
		SFIELD_INFO(wtf, float, TestComponentWrap, meta)
		SFIELD_INFO(compPtr, decltype(&TestComponentWrap::compPtr), TestComponentWrap, nullptr)
		return hana::make_tuple(comp_info(), wtf_info(), compPtr_info());
	}

	inline static constexpr auto staticAttrib_tup = hana::make_tuple(Field{
			"staticAttrib", "TestComponent", 0, {nullptr}
		}, &TestComponentWrap::statComp);
	inline static constexpr auto all_static_tup = hana::make_tuple(staticAttrib_tup);
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
	SClass<std::decay<T>::type>::ForEachFieldMeta(
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
}

int main(void)
{
	printFieldMeta<TestComponentWrap>();
	TestComponent testComp;
	ClassInfo<TestComponent>::for_each_field(testComp,
		Sakura::overload(
			[](const std::string& field, const Field& meta) {
				std::cout << "string: " << meta.name << ": " << field << std::endl;
			},
			[](auto&& field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			}));

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
				(std::is_same<std::decay<decltype(method)>::type, void(TestComponent::*)()>())
			{
				std::cout << meta.name << ": " << meta.type << std::endl;
				(testComp.*method)();
			}
		});
	return 0;
}
