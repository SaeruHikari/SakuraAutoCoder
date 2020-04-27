/*
 * @Author: your name
 * @Date: 2020-04-16 16:26:13
 * @LastEditTime: 2020-04-28 01:05:12
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
		void gen_meta(code_generator::output& output, std::string prefix,
			const std::unordered_map<std::string, std::string>& data);

		inline void generate_var_meta(code_generator::output& output,
			const cpp_entity& var, const ReflField& field)
		{
			gen_meta(output, var.name() + "_", field.fieldMetas);
		}

		void collect_field_meta(
			ReflField& field, const cppast::cpp_entity& member);
			
		void collect_static_field_meta(
			ReflField& field, const cppast::cpp_entity& member);
	}

	using cpp_member_variable = cppast::cpp_member_variable;

	inline bool generate_meta_class_member_varable(code_generator::output& output,
		const cpp_entity& var, const ReflField& field)
	{
		detail::generate_var_meta(output, var, field);
		return true;
	}
}