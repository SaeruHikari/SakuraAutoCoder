#include <refl.rule.hxx>

using namespace Sakura::refl;

template<>
struct ClassInfo<TestComponent>
{
inline static const constexpr const char* GetClassName() 
{
    return "TestComponent";
}
inline static const constexpr Meta::MetaPiece meta[2] = 
{
    {"refl", "null"},
    {"descriptions", "This is a test component"}
};
inline static const constexpr Meta::MetaPiece attrib_meta[1] = 
{
    {"meta", "SaeruHikari"}
};

inline static const constexpr Meta::MetaPiece name_meta[1] = 
{
    {"meta", "Fxxk ISO C++23"}
};

inline static const constexpr Meta::MetaPiece staticAttrib_meta[1] = 
{
    {"meta", "This is a static attribute"}
};

inline static const constexpr Meta::MetaPiece Method_meta[1] = 
{
    {"meta", "Can't wait to use compile-time-reflection"}
};

inline static const constexpr auto all_fields()
{
    SFIELD_INFO(attrib, float, TestComponent, attrib_meta);
    SFIELD_INFO(name, std::string, TestComponent, name_meta);
    return hana::make_tuple(attrib_info(), name_info());
}
inline static const constexpr auto all_static_fields()
{
    SSTATICFIELD_INFO(staticAttrib, float const, TestComponent, staticAttrib_meta);
    return hana::make_tuple(staticAttrib_info());
}
inline static const constexpr auto all_methods()
{
    SMETHOD_INFO(Method, method, TestComponent, Method_meta);
    return hana::make_tuple(Method_info());
}

};
template<>
struct ClassInfo<TestComponentWrap>
{
inline static const constexpr const char* GetClassName() 
{
    return "TestComponentWrap";
}
inline static const constexpr Meta::MetaPiece meta[1] = 
{
    {"refl", "null"}
};
inline static const constexpr Meta::MetaPiece comp_meta[1] = 
{
    {"description", "Test wrapping of reflection component"}
};



inline static const constexpr auto all_fields()
{
    SFIELD_INFO(comp, TestComponent, TestComponentWrap, comp_meta);
    SFIELD_INFO(wtf, float, TestComponentWrap, nullptr);
    return hana::make_tuple(comp_info(), wtf_info());
}
inline static const constexpr auto all_static_fields()
{
    SSTATICFIELD_INFO(statComp, TestComponent const, TestComponentWrap, nullptr);
    return hana::make_tuple(statComp_info());
}

};
