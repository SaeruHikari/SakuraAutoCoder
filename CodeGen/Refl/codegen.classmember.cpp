/*
 * @Author: your name
 * @Date: 2020-04-28 00:26:59
 * @LastEditTime: 2020-04-28 01:54:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/codegen.classmember.cpp
 */
#include "codegen.classmember.hpp"
#include <vector>
#include <iterator>
#include <iostream>

using namespace Sakura::refl;
extern std::unordered_map<std::string, int> non_meta_attributes;

void Sakura::refl::detail::collect_static_field_meta(
    ReflField& field, const cppast::cpp_entity& member)
{
    field.name = member.name().c_str();
    field.type
        = cppast::to_string(((const cppast::cpp_variable&)member).type());
    for (auto i = 0; i < member.attributes().size(); i++)
    {
        std::pair<std::string, std::string> attrib{ member.attributes()[i].name() , "" };
        if (member.attributes()[i].arguments().has_value())
            attrib.second = member.attributes()[i].arguments().value().as_string();
        field.fieldMetas.insert(attrib);
    }
}

void Sakura::refl::detail::collect_field_meta(
    ReflField& field, const cppast::cpp_entity& member)
{
    field.name = member.name().c_str();
    field.type
        = cppast::to_string(((const cpp_member_variable&)member).type());
    for (auto i = 0; i < member.attributes().size(); i++)
    {
        std::pair<std::string, std::string> attrib{ member.attributes()[i].name() , "" };
        if (member.attributes()[i].arguments().has_value())
            attrib.second = member.attributes()[i].arguments().value().as_string();
        field.fieldMetas.insert(attrib);
    }
}

void Sakura::refl::detail::gen_meta(code_generator::output& output,
    std::string prefix, const std::unordered_map<std::string, std::string>& data)
{
    if (data.size() > 0)
    {
        auto i = 0u;
        for (auto iter = data.begin(); iter != data.end(); iter++)
        {
            if(non_meta_attributes.find(iter->first) 
                == non_meta_attributes.end())
            i++;
        }
        if(i > 0)
        {
            output << punctuation("\t");
            detail::inline_static_const_constexpr(output);
            output << identifier("auto") << cppast::whitespace << identifier(prefix + "meta")
                << punctuation(" = \n\tSakura::unordered_map_c<std::string_view, std::string_view>({");
            for (auto iter = data.begin(); iter != data.end(); iter++)
            {
                if(non_meta_attributes.find(iter->first) 
                    == non_meta_attributes.end())
                {
                    output << punctuation("\n\t\t{") 
                        << cppast::string_literal("\"" + iter->first + "\"sv")
                        << punctuation(", ") 
                        << ((iter->second.size() > 0) ?
                        cppast::string_literal(iter->second) : 
                        cppast::string_literal("\"null\"sv"))
                        << punctuation("}");
                    i--;
                    if (i != 0)
                        output << punctuation(",");
                }
            }
            output << punctuation("\n\t});\n\n");
        }
    }
}