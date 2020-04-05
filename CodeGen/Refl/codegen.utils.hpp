#pragma once
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_file.hpp>


namespace Sakura::refl
{
	using cpp_entity = cppast::cpp_entity;
	using cpp_entity_kind = cppast::cpp_entity_kind;
	using keyword = cppast::keyword;
	using formatting_flags = cppast::formatting_flags;
	using punctuation = cppast::punctuation;
	using code_generator = cppast::code_generator;
	using cpp_class = cppast::cpp_class;
	using cpp_base_class = cppast::cpp_base_class;
	using cpp_file = cppast::cpp_file;
	using cpp_access_specifier_kind = cppast::cpp_access_specifier_kind;
	using cpp_template_specialization = cppast::cpp_template_specialization;
	using cpp_template_argument = cppast::cpp_template_argument;
	using output = code_generator::output;

	namespace detail
	{
		inline void comma(const code_generator::output& output)
		{
			output << punctuation(",");
			if (output.formatting().is_set(formatting_flags::comma_ws))
				output << cppast::whitespace;
		}

		inline void bracket_ws(const code_generator::output& output)
		{
			if (output.formatting().is_set(formatting_flags::bracket_ws))
				output << cppast::whitespace;
		}

		inline void operator_ws(const code_generator::output& output)
		{
			if (output.formatting().is_set(formatting_flags::operator_ws))
				output << cppast::whitespace;
		}

		inline void opening_brace(const code_generator::output& output)
		{
			if (output.formatting().is_set(formatting_flags::brace_nl))
				output << cppast::newl;
			else if (output.formatting().is_set(formatting_flags::brace_ws))
				output << cppast::whitespace;
			output << punctuation("{");
		}

		inline void write_access_specifier(code_generator::output& output,
			cpp_access_specifier_kind access)
		{
			output.unindent();
			output << keyword(to_string(access)) << punctuation(":");
			output.indent();
		}
	}

	template <class Container, typename Sep>
	inline bool write_container(code_generator::output& output, const Container& cont, Sep s,
		cpp_access_specifier_kind cur_access,
		std::function<bool(cppast::code_generator&,
			const cpp_entity&, cpp_access_specifier_kind)> codegenImpl)
	{
		auto need_sep = false;
		for (auto& child : cont)
		{
			auto is_excluded = output.options(child, cur_access).is_set(code_generator::exclude);
			if (!is_excluded)
			{
				if (need_sep)
					output << s;
				need_sep = codegenImpl(*output.generator(), child, cur_access);
			}
		}
		return need_sep;
	}
}