/*
 * @Author: your name
 * @Date: 2020-04-16 16:26:13
 * @LastEditTime: 2020-04-27 17:27:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/codegen.classmember.hpp
 */
#pragma once
#include "codegen.utils.hpp"
#include <cppast/cpp_variable.hpp>
#include <string>

namespace Sakura::refl
{
	namespace detail
	{
		inline void gen_meta(code_generator::output& output, std::string prefix,
			const std::unordered_map<std::string, std::string>& data)
		{
			if (data.size() > 0)
			{
				output << punctuation("\t");
				detail::inline_static_const_constexpr(output);
				output << identifier("auto") << cppast::whitespace << identifier(prefix + "meta")
					<< punctuation(" = \n\tSakura::unordered_map_c<std::string_view, std::string_view>({");
				auto i = 0u;
				for (auto iter = data.begin(); iter != data.end(); iter++)
				{
					i++;
					output << punctuation("\n\t\t{") 
						<< cppast::string_literal("\"" + iter->first + "\"sv")
						<< punctuation(", ") 
						<< ((iter->second.size() > 0) ?
						cppast::string_literal(iter->second) : 
						cppast::string_literal("\"null\"sv"))
						<< punctuation("}");
					if (i != data.size())
						output << punctuation(",");
				}
				output << punctuation("\n\t});\n\n");
			}
		}

		inline void generate_var_meta(code_generator::output& output,
			const cpp_entity& var, const ReflField& field)
		{
			gen_meta(output, var.name() + "_", field.fieldMetas);
		}

		inline void collect_field_meta(ReflField& field, const cppast::cpp_entity& member)
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

		inline void collect_static_field_meta(ReflField& field, const cppast::cpp_entity& member)
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
	}

	using cpp_member_variable = cppast::cpp_member_variable;

	inline bool generate_meta_class_member_varable(code_generator::output& output,
		const cpp_entity& var, const ReflField& field)
	{
		detail::generate_var_meta(output, var, field);
		return true;
	}
}