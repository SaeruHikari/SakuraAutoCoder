/*
 * @Author: your name
 * @Date: 2020-04-24 23:23:46
 * @LastEditTime: 2020-04-28 00:22:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/codegem.class.cpp
 */
#include "codegen.enum.hpp"
#include "codegen.utils.hpp"
#include "codegen.classmember.hpp"
#include <cppast/cpp_enum.hpp>
#include <iostream>

using namespace Sakura::refl;

extern bool bDebugAST;
extern std::unordered_map<std::string, Sakura::refl::ReflUnit> ReflUnits;

auto get_enum_entity_Name_sp(const cppast::cpp_enum& c)
{
    const cpp_entity* unit = &c;
    std::string _n = c.name();
    std::string _t = "::";
    while(unit->parent().has_value() &&
        (unit->parent().value().kind() == cpp_entity_kind::namespace_t ||
            unit->parent().value().kind() == cpp_entity_kind::class_t))
    {
        _n = unit->parent().value().name() + _t + _n;
        unit = &unit->parent().value();
    }
    return _n;
}

void gen_getEnumName(code_generator::output& output, const cppast::cpp_enum& c)
{
    output << punctuation("\t");
    detail::inline_static_const_constexpr(output);
    output << keyword("const") << cppast::whitespace << cppast::identifier("std::string_view")
        << cppast::whitespace << identifier("GetEnumName") << punctuation("()") 
        << cppast::newl << cppast::punctuation("\n\t{\n");
    output << cppast::punctuation("\t\t") << keyword("return") << cppast::whitespace <<
        cppast::string_literal("\"") <<
            cppast::string_literal(get_enum_entity_Name_sp(c))
            << cppast::string_literal("\"") << cppast::punctuation(";\n\t}\n");
}

    inline void collect_enum_value_meta(ReflField& field, const cppast::cpp_entity& member)
    {
        field.name = member.name().c_str();
        field.type = "enum_value";
        for (auto i = 0; i < member.attributes().size(); i++)
        {
            std::pair<std::string, std::string> attrib{ member.attributes()[i].name() , "" };
            if (member.attributes()[i].arguments().has_value())
                attrib.second = member.attributes()[i].arguments().value().as_string();
            field.fieldMetas.insert(attrib);
        }
    }

bool Sakura::refl::generate_enum(
        std::function<bool(cppast::code_generator&,
            const cpp_entity&, cpp_access_specifier_kind)> codegenImpl,
        code_generator& generator, const cppast::cpp_enum& e,
        cpp_access_specifier_kind cur_access,
        type_safe::optional_ref<const cpp_template_specialization> spec)
    {
        code_generator::output output(type_safe::ref(generator), type_safe::ref(e), cur_access);
		ReflUnit reflUnit;
		if (output)
		{
            using namespace cppast;
            reflUnit.unitName = get_enum_entity_Name_sp(e);
            for (auto i = 0; i < e.attributes().size(); i++)
			{
				std::pair<std::string, std::string> attrib{ e.attributes()[i].name() , "" };
				if (e.attributes()[i].arguments().has_value())
					attrib.second = e.attributes()[i].arguments().value().as_string();
				reflUnit.unitMetas.insert(attrib);
			}
            if(reflUnit.unitMetas.find("refl") != reflUnit.unitMetas.end())
			{
				output << identifier("template<>\nstruct EnumInfo<" + 
					get_enum_entity_Name_sp(e) + ">\n{\n");
			}
            else return true;
            gen_getEnumName(output, e);
            detail::gen_meta(output, "", reflUnit.unitMetas);
            for (auto& member : e)
			{
                collect_enum_value_meta(reflUnit.staticFieldsMap[member.name()], member);
                generate_meta_class_member_varable(output, member,
					reflUnit.staticFieldsMap[member.name()]);
            }
            detail::gen_all_fields(output, reflUnit, detail::FieldSet::E_ENUM_VALUES);
            output << punctuation("};\n\n");
            ReflUnits[e.name()] = reflUnit;
        }
        return true;
    }