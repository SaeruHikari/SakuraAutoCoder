/*
 * @Author: your name
 * @Date: 2020-04-24 23:16:11
 * @LastEditTime: 2020-04-24 23:20:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/codegen.enum.hpp
 */
#pragma once
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_class_template.hpp>
#include <cppast/code_generator.hpp>
#include "codegen.utils.hpp"

using namespace std;

namespace Sakura::refl
{
    bool generate_enum(
		std::function<bool(cppast::code_generator&,
			const cpp_entity&, cpp_access_specifier_kind)> codegenImpl,
		code_generator& generator, const cppast::cpp_enum& c,
		cpp_access_specifier_kind cur_access,
		type_safe::optional_ref<const cpp_template_specialization> spec = nullptr);
} 