/*
 * @Author: your name
 * @Date: 2020-04-16 16:26:13
 * @LastEditTime: 2020-04-16 21:06:12
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/codegen.classmember.hpp
 */
#pragma once
#include "codegen.utils.hpp"
#include <string>

namespace Sakura::refl
{
	struct ReflField
	{
		std::string name;//8
		std::string type;//8 16
		uint32_t offset;// 4 24
		std::unordered_map<std::string, std::string> fieldMetas = {};
	};
	struct ReflUnit
	{
		std::string unitName;
		std::unordered_map<std::string, std::string> unitMetas;
		std::unordered_map<std::string, ReflField> fieldsMap;
		std::unordered_map<std::string, ReflField> staticFieldsMap;
		std::unordered_map<std::string, ReflField> methodsMap;
		std::unordered_map<std::string, ReflField> staticMethodsMap;
	};

	namespace detail
	{
		inline void gen_meta(code_generator::output& output, std::string prefix,
			const std::unordered_map<std::string, std::string>& data)
		{
			if (data.size() > 0)
			{
				detail::inline_static_const_constexpr(output);
				output << identifier("Meta::MetaPiece") << cppast::whitespace << identifier(prefix + "meta")
					<< punctuation("[") << cppast::int_literal(std::to_string(data.size())) << punctuation("] = \n{");
				auto i = 0u;
				for (auto iter = data.begin(); iter != data.end(); iter++)
				{
					i++;
					output << punctuation("\n    {") 
						<< cppast::string_literal("\"" + iter->first + "\"")
						<< punctuation(", ") 
						<< ((iter->second.size() > 0) ?
						cppast::string_literal(iter->second) : 
						cppast::string_literal("\"null\""))
						<< punctuation("}");
					if (i != data.size())
						output << punctuation(",");
				}
				output << punctuation("\n};\n");
			}
		}

		inline void generate_var_meta(code_generator::output& output,
			const cpp_member_variable& var, const ReflField& field)
		{
			gen_meta(output, var.name() + "_", field.fieldMetas);
		}
	}

	using cpp_member_variable = cppast::cpp_member_variable;

	inline bool generate_meta_class_member_varable(code_generator::output& output,
		const cpp_member_variable& var, const ReflField& field)
	{
		detail::generate_var_meta(output, var, field);
		output << cppast::punctuation("\n");
		return true;
	}
}