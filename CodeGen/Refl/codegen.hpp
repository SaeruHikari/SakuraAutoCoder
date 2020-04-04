#include <cppast/code_generator.hpp>
#include <cppast/cpp_alias_template.hpp>
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_class_template.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_enum.hpp>
#include <cppast/cpp_file.hpp>
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

using namespace std;

namespace Sakura::refl
{
	using code_generator =  cppast::code_generator;
	using cpp_entity =  cppast::cpp_entity;
	using cpp_access_specifier_kind =  cppast::cpp_access_specifier_kind;
	using cpp_file =  cppast::cpp_file;
	using cpp_entity_kind =  cppast::cpp_entity_kind;
	using cpp_template_specialization =  cppast::cpp_template_specialization;
	using cpp_template_argument = cppast::cpp_template_argument;
	using cpp_class =  cppast::cpp_class;
	using cpp_base_class =  cppast::cpp_base_class;
	using punctuation =  cppast::punctuation;
	using output = code_generator::output;
	using formatting_flags = cppast::formatting_flags;
	using keyword = cppast::keyword;

	bool generate_code_impl(cppast::code_generator& generator, const cpp_entity& e,
		cpp_access_specifier_kind cur_access);

	namespace detail
	{
		void comma(const code_generator::output& output)
		{
			output << punctuation(",");
			if (output.formatting().is_set(formatting_flags::comma_ws))
				output << cppast::whitespace;
		}

		void bracket_ws(const code_generator::output& output)
		{
			if (output.formatting().is_set(formatting_flags::bracket_ws))
				output << cppast::whitespace;
		}

		void operator_ws(const code_generator::output& output)
		{
			if (output.formatting().is_set(formatting_flags::operator_ws))
				output << cppast::whitespace;
		}

		void opening_brace(const code_generator::output& output)
		{
			if (output.formatting().is_set(formatting_flags::brace_nl))
				output << cppast::newl;
			else if (output.formatting().is_set(formatting_flags::brace_ws))
				output << cppast::whitespace;
			output << punctuation("{");
		}

		void write_access_specifier(code_generator::output& output, cpp_access_specifier_kind access)
		{
			output.unindent();
			output << keyword(to_string(access)) << punctuation(":");
			output.indent();
		}

		bool generate_base_class(code_generator& generator, const cpp_base_class& base,
			cpp_access_specifier_kind)
		{
			DEBUG_ASSERT(base.parent() && base.parent().value().kind() == cpp_entity_kind::class_t,
				cppast::detail::assert_handler{});
			auto parent_kind = static_cast<const cpp_class&>(base.parent().value()).class_kind();

			code_generator::output output(type_safe::ref(generator), type_safe::ref(base),
				base.access_specifier());
			if (output)
			{
				if (base.is_virtual())
					output << cppast::keyword("virtual") << cppast::whitespace;

				auto access = base.access_specifier();
				if (access == cppast::cpp_protected)
					output << cppast::keyword("protected") << cppast::whitespace;
				else if (access == cppast::cpp_private && parent_kind !=
					cppast::cpp_class_kind::class_t)
					output << cppast::keyword("private") << cppast::whitespace;
				else if (access == cppast::cpp_public && 
						parent_kind == cppast::cpp_class_kind::class_t)
					output << cppast::keyword("public") << cppast::whitespace;

				output << cppast::identifier(base.name());
			}
			return static_cast<bool>(output);
		}

		void write_bases(code_generator& generator, code_generator::output& output, const cpp_class& c)
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

		void write_template_arguments(
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
	}


	template <class Container, typename Sep>
	bool write_container(code_generator::output& output, const Container& cont, Sep s,
		cpp_access_specifier_kind cur_access)
	{
		auto need_sep = false;
		for (auto& child : cont)
		{
			auto is_excluded = output.options(child, cur_access).is_set(code_generator::exclude);
			if (!is_excluded)
			{
				if (need_sep)
					output << s;
				need_sep = generate_code_impl(*output.generator(), child, cur_access);
			}
		}
		return need_sep;
	}

	bool generate_file(code_generator& generator, const cpp_file& f,
		cpp_access_specifier_kind cur_access)
	{
		code_generator::output output(type_safe::ref(generator), type_safe::ref(f), cur_access);
		if (output)
		{
			auto need_sep = write_container(output, f, cppast::newl, cur_access);
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

	void write_specialization_arguments(code_generator::output& output,
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

	bool generate_class(code_generator& generator, const cpp_class& c,
		cpp_access_specifier_kind                                  cur_access,
		type_safe::optional_ref<const cpp_template_specialization> spec = nullptr)
	{
		using namespace cppast;
		code_generator::output output(type_safe::ref(generator), type_safe::ref(c), cur_access);
		if (output)
		{
			if (is_friended(c))
				output << keyword("friend") << whitespace;
			output << keyword(to_string(c.class_kind())) << whitespace;

			output << identifier(c.semantic_scope());
			if (spec)
			{
				output << spec.value().primary_template();
				write_specialization_arguments(output, spec.value());
			}
			else
				output << identifier(c.name());

			if (c.is_final())
				output << whitespace << keyword("final");

			if (!output.generate_definition() || c.is_declaration())
				output << punctuation(";") << newl;
			else
			{
				detail::write_bases(generator, output, c);
				output << detail::opening_brace;
				output.indent();

				auto need_sep = false;
				auto last_access = c.class_kind() == cpp_class_kind::class_t ? cpp_private : cpp_public;
				auto last_written_access = last_access;
				for (auto& member : c)
				{
					if (member.kind() == cpp_entity_kind::access_specifier_t)
					{
						auto& access = static_cast<const cpp_access_specifier&>(member);
						last_access = access.access_specifier();
					}
					else if (output.options(member, last_access).is_set(code_generator::exclude))
						continue;
					else
					{
						if (need_sep)
							output << newl;
						if (last_access != last_written_access)
						{
							detail::write_access_specifier(output, last_access);
							last_written_access = last_access;
						}
						need_sep = generate_code_impl(generator, member, last_access);
					}
				}

				output.container_end();

				output.unindent();
				output << punctuation("};") << newl;
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
			generator, static_cast<const cpp_##Name&>(e), cur_access);

			CPPAST_DETAIL_HANDLE(file)
			CPPAST_DETAIL_HANDLE(class)
		}
		return true;
	}
}