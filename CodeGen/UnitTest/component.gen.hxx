//generated time:1587909050
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
	inline static const constexpr Meta::MetaPiece meta[2] = 
	{
		{"descriptions", "This is a test component"},
		{"refl", "null"}
	};

	inline static const constexpr Meta::MetaPiece attrib_meta[2] = 
	{
		{"meta", "SaeruHikari"},
		{"refl", "null"}
	};

	inline static const constexpr Meta::MetaPiece testContainer_meta[2] = 
	{
		{"container", "This is a container!"},
		{"refl", "null"}
	};

	inline static const constexpr Meta::MetaPiece staticAttrib_meta[2] = 
	{
		{"meta", "This is a static attribute"},
		{"refl", "null"}
	};

	inline static const constexpr Meta::MetaPiece Method_meta[2] = 
	{
		{"meta", "Can't wait to use compile-time-reflection"},
		{"refl", "null"}
	};

	SFIELD_INFO(attrib, Test::TestComponent, attrib_meta);
	SFIELD_INFO(testContainer, Test::TestComponent, testContainer_meta);
	inline static const constexpr auto all_fields()
	{
		return hana::make_tuple(attrib_info(), testContainer_info());
	}
	SSTATICFIELD_INFO(staticAttrib, Test::TestComponent, staticAttrib_meta);
	inline static const constexpr auto all_static_fields()
	{
		return hana::make_tuple(staticAttrib_info());
	}
	SMETHOD_INFO(Method, Test::TestComponent, Method_meta);
	inline static const constexpr auto all_methods()
	{
		return hana::make_tuple(Method_info());
	}
};

template<>
struct ClassInfo<Test::TestComponentWrap>
{
	inline static const constexpr const char* GetClassName() 
	{
		return "Test::TestComponentWrap";
	}
	inline static const constexpr Meta::MetaPiece meta[1] = 
	{
		{"refl", "null"}
	};

	inline static const constexpr Meta::MetaPiece comp_meta[2] = 
	{
		{"description", "Test wrapping of reflection component"},
		{"refl", "null"}
	};

	SFIELD_INFO(comp, Test::TestComponentWrap, comp_meta);
	inline static const constexpr auto all_fields()
	{
		return hana::make_tuple(comp_info());
	}
};

template<>
struct EnumInfo<Test::TestEnum>
{
	inline static const constexpr const char* GetEnumName() 
	{
		return "Test::TestEnum";
	}
	inline static const constexpr Meta::MetaPiece meta[1] = 
	{
		{"refl", "null"}
	};

	inline static const constexpr Meta::MetaPiece E_ONE_meta[1] = 
	{
		{"enum", "This is enum one"}
	};

	inline static const constexpr Meta::MetaPiece E_TWO_meta[1] = 
	{
		{"enum", "This is enum two"}
	};

	inline static const constexpr Meta::MetaPiece E_THREE_meta[1] = 
	{
		{"enum", "This is enum three"}
	};

	SENUM_FIELD_INFO(E_ONE, Test::TestEnum, E_ONE_meta);
	SENUM_FIELD_INFO(E_TWO, Test::TestEnum, E_TWO_meta);
	SENUM_FIELD_INFO(E_THREE, Test::TestEnum, E_THREE_meta);
	inline static const constexpr auto all_static_fields()
	{
		return hana::make_tuple(E_ONE_info(), E_TWO_info(), E_THREE_info());
	}
};

