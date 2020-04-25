/*
 * @Author: your name
 * @Date: 2020-04-22 00:49:46
 * @LastEditTime: 2020-04-25 02:33:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/codegen.hpp
 */
#pragma once
#include <cppast/cpp_alias_template.hpp>
#include <cppast/cpp_enum.hpp>
#include <cppast/cpp_friend.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_function_template.hpp>
#include <cppast/cpp_language_linkage.hpp>
#include <cppast/cpp_member_function.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_namespace.hpp>
#include <cppast/cpp_preprocessor.hpp>
#include <cppast/cpp_static_assert.hpp>
#include <cppast/cpp_template_parameter.hpp>
#include <cppast/cpp_token.hpp>
#include <cppast/cpp_type_alias.hpp>
#include <cppast/cpp_variable.hpp>
#include <cppast/cpp_variable_template.hpp>
#include "codegen.class.hpp"
#include "codegen.enum.hpp"
using namespace std;

namespace Sakura::refl
{
	bool generate_code_impl(cppast::code_generator& generator, const cpp_entity& e,
		cpp_access_specifier_kind cur_access);

	inline bool generate_file(
		std::function<bool(cppast::code_generator&,
			const cpp_entity&, cpp_access_specifier_kind)> codegenImpl,
		code_generator& generator, const cpp_file& f,
		cpp_access_specifier_kind cur_access)
	{
		code_generator::output output(type_safe::ref(generator), type_safe::ref(f), cur_access);
		if (output)
		{
			auto need_sep = write_container(output, f, cppast::newl, cur_access, codegenImpl);
			if (!need_sep)
			{
				output << cppast::newl;
			}
			else
			{
				output.container_end();
			}
		}
		return static_cast<bool>(output);
	}

	bool generate_code_impl(cppast::code_generator& generator, const cpp_entity& e,
		cpp_access_specifier_kind cur_access)
	{
		switch (e.kind())
		{
#define CPPAST_DETAIL_HANDLE(Name)                                                                 \
    case cpp_entity_kind::Name##_t:                                                                \
        return Sakura::refl::generate_##Name(\
			generate_code_impl, generator, static_cast<const cppast::cpp_##Name&>(e), cur_access);

			CPPAST_DETAIL_HANDLE(file)
			CPPAST_DETAIL_HANDLE(class)
			CPPAST_DETAIL_HANDLE(enum)
		}
		return true;
	}
}