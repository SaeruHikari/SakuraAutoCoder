//generated time:1588079707
//generated end


#pragma once
#include <refl.rule.hxx>

using namespace Sakura::Refl;

template<> inline const constexpr bool Sakura::Refl::isAtomic<Test::wtf>(){return true;}
template<>
struct ClassInfo<Test::TestComponent>
{
	inline static const constexpr const std::string_view GetClassName() 
	{
		return Sakura::Refl::decay_type_name<Test::TestComponent>();
	}
	inline static const constexpr const std::string_view GetPrettyName() 
	{
		return "Test Component";
	}
	inline static const constexpr auto meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"descriptions"sv, "This is a test component"}
	});

	inline static const constexpr auto attrib_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"meta"sv, "SaeruHikari"}
	});

	inline static const constexpr auto testContainer_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"container"sv, "This is a container!"}
	});

	inline static const constexpr auto name_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"meta"sv, "Fxxk ISO C++23"}
	});

	inline static const constexpr auto staticAttrib_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"meta"sv, "This is a static attribute"}
	});

	inline static const constexpr auto Method_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"meta"sv, "Can't wait to use compile-time-reflection"}
	});

	SFIELD_INFO(attrib, Test::TestComponent, attrib_meta);
	SFIELD_INFO(name, Test::TestComponent, name_meta);
	SFIELD_INFO(testContainer, Test::TestComponent, testContainer_meta);
	inline static const constexpr auto all_fields()
	{
		return Sakura::make_tuple(attrib_info(), name_info(), testContainer_info());
	}
	SSTATICFIELD_INFO(staticAttrib, Test::TestComponent, staticAttrib_meta);
	inline static const constexpr auto all_static_fields()
	{
		return Sakura::make_tuple(staticAttrib_info());
	}
	SMETHOD_INFO(Method, Test::TestComponent, Method_meta);
	inline static const constexpr auto all_methods()
	{
		return Sakura::make_tuple(Method_info());
	}
};
SERIALIZE_IMPLEMENTATION_DEFAULT(Test::TestComponent);

template<>
struct ClassInfo<Test::TestComponentWrap>
{
	inline static const constexpr const std::string_view GetClassName() 
	{
		return Sakura::Refl::decay_type_name<Test::TestComponentWrap>();
	}
	inline static const constexpr const std::string_view GetPrettyName() 
	{
		return "Test::TestComponentWrap";
	}
	inline static const constexpr auto comp_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"description"sv, "Test wrapping of reflection component"}
	});

	inline static const constexpr auto statComp_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"st"sv, "OHHHHHH"}
	});

	inline static const constexpr auto wtf_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"wf"sv, "ddd"}
	});

	SFIELD_INFO(comp, Test::TestComponentWrap, comp_meta);
	SFIELD_INFO(wtf, Test::TestComponentWrap, wtf_meta);
	inline static const constexpr auto all_fields()
	{
		return Sakura::make_tuple(comp_info(), wtf_info());
	}
	SSTATICFIELD_INFO(statComp, Test::TestComponentWrap, statComp_meta);
	inline static const constexpr auto all_static_fields()
	{
		return Sakura::make_tuple(statComp_info());
	}
};
SERIALIZE_IMPLEMENTATION_DEFAULT(Test::TestComponentWrap);

template<>
struct EnumInfo<Test::TestEnum>
{
	inline static const constexpr const std::string_view GetEnumName() 
	{
		return "Test::TestEnum";
	}
	inline static const constexpr auto E_ONE_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"enum"sv, "This is enum one"}
	});

	inline static const constexpr auto E_TWO_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"enum"sv, "This is enum two"}
	});

	inline static const constexpr auto E_THREE_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"enum"sv, "This is enum three"}
	});

	SENUM_FIELD_INFO(E_ONE, Test::TestEnum, E_ONE_meta);
	SENUM_FIELD_INFO(E_TWO, Test::TestEnum, E_TWO_meta);
	SENUM_FIELD_INFO(E_THREE, Test::TestEnum, E_THREE_meta);
	inline static const constexpr auto all_static_fields()
	{
		return Sakura::make_tuple(E_ONE_info(), E_TWO_info(), E_THREE_info());
	}
};

