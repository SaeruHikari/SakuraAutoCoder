//generated time:1587979927
//generated end


#pragma once
#include <refl.rule.hxx>

using namespace Sakura::refl;

template<>
struct ClassInfo<Test::TestComponent>
{
	inline static const constexpr const char* GetClassName() 
	{
		return "Test::TestComponent";
	}
	inline static const constexpr auto meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"descriptions"sv, "This is a test component"},
		{"refl"sv, "null"sv}
	});

	inline static const constexpr auto testContainer_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"container"sv, "This is a container!"},
		{"refl"sv, "null"sv}
	});

	inline static const constexpr auto staticAttrib_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"meta"sv, "This is a static attribute"},
		{"refl"sv, "null"sv}
	});

	inline static const constexpr auto Method_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"meta"sv, "Can't wait to use compile-time-reflection"},
		{"refl"sv, "null"sv}
	});

	SFIELD_INFO(testContainer, Test::TestComponent, testContainer_meta);
	inline static const constexpr auto all_fields()
	{
		return std::make_tuple(testContainer_info());
	}
	SSTATICFIELD_INFO(staticAttrib, Test::TestComponent, staticAttrib_meta);
	inline static const constexpr auto all_static_fields()
	{
		return std::make_tuple(staticAttrib_info());
	}
	SMETHOD_INFO(Method, Test::TestComponent, Method_meta);
	inline static const constexpr auto all_methods()
	{
		return std::make_tuple(Method_info());
	}
};

template<>
struct ClassInfo<Test::TestComponentWrap>
{
	inline static const constexpr const char* GetClassName() 
	{
		return "Test::TestComponentWrap";
	}
	inline static const constexpr auto meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"refl"sv, "null"sv}
	});

	inline static const constexpr auto comp_meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"description"sv, "Test wrapping of reflection component"},
		{"refl"sv, "null"sv}
	});

	SFIELD_INFO(comp, Test::TestComponentWrap, comp_meta);
	inline static const constexpr auto all_fields()
	{
		return std::make_tuple(comp_info());
	}
};

template<>
struct EnumInfo<Test::TestEnum>
{
	inline static const constexpr const char* GetEnumName() 
	{
		return "Test::TestEnum";
	}
	inline static const constexpr auto meta = 
	Sakura::unordered_map_c<std::string_view, std::string_view>({
		{"refl"sv, "null"sv}
	});

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
		return std::make_tuple(E_ONE_info(), E_TWO_info(), E_THREE_info());
	}
};

