/*
 * @Author: your name
 * @Date: 2020-04-22 00:49:46
 * @LastEditTime: 2020-04-24 23:16:32
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/codegen.class.hpp
 */
#pragma once
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_class_template.hpp>
#include <cppast/code_generator.hpp>
#include "codegen.utils.hpp"
#include "codegen.classmember.hpp"

using namespace std;

namespace Sakura::refl
{
	namespace detail
	{
		bool generate_base_class(code_generator& generator, const cpp_base_class& base,
			cpp_access_specifier_kind);
		
		static void gen_getClassName(code_generator::output& output, const cpp_class& c);

		inline void write_template_arguments(
			code_generator::output& output,
			type_safe::optional<type_safe::array_ref<const cpp_template_argument>> arguments)
		{
			if (!arguments)
			{
				output << punctuation("<") << punctuation(">");
			}
			else
			{
				output << punctuation("<") << bracket_ws;
				auto need_sep = false;
				for (auto& arg : arguments.value())
				{
					if (need_sep)
						output << comma;
					else
						need_sep = true;

					if (auto type = arg.type())
						cppast::detail::write_type(output, type.value(), "");
					else if (auto expr = arg.expression())
						cppast::detail::write_expression(output, expr.value());
					else if (auto templ = arg.template_ref())
						output << templ.value();
					else
						DEBUG_UNREACHABLE(cppast::detail::assert_handler{});
				}
				output << bracket_ws << punctuation(">");
			}
		}

		inline void write_bases(code_generator& generator,
			code_generator::output& output, const cpp_class& c)
		{
			auto need_sep = false;
			auto first = true;
			for (auto& base : c.bases())
			{
				auto opt = output.options(base, base.access_specifier());
				if (first && !opt.is_set(code_generator::exclude))
				{
					first = false;
					output << cppast::newl << punctuation(":") << operator_ws;
				}
				else if (need_sep)
					output << comma;
				need_sep = generate_base_class(generator, base, cppast::cpp_public);
			}
		}
	}

	inline void write_specialization_arguments(code_generator::output& output,
		const cpp_template_specialization& spec)
	{
		if (spec.arguments_exposed())
			detail::write_template_arguments(output, spec.arguments());
		else if (!spec.unexposed_arguments().empty())
		{
			output << punctuation("<") << detail::bracket_ws;
			cppast::detail::write_token_string(output, spec.unexposed_arguments());
			output << detail::bracket_ws << punctuation(">");
		}
	}

	bool generate_class(
		std::function<bool(cppast::code_generator&,
			const cpp_entity&, cpp_access_specifier_kind)> codegenImpl,
		code_generator& generator, const cpp_class& c,
		cpp_access_specifier_kind cur_access,
		type_safe::optional_ref<const cpp_template_specialization> spec = nullptr);
}