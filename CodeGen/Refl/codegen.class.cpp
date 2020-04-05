#include "codegen.class.hpp"
#include <cppast/cpp_alias_template.hpp>
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

namespace Sakura::refl
{
	namespace detail
	{
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
	}

	bool generate_class(
		std::function<bool(cppast::code_generator&,
			const cpp_entity&, cpp_access_specifier_kind)> codegenImpl,
		code_generator& generator, const cpp_class& c,
		cpp_access_specifier_kind                                  cur_access,
		type_safe::optional_ref<const cpp_template_specialization> spec)
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
			{
				output << identifier("template<>SClass<" + c.name() + "> final : public IClass{");
			}

			//if (c.is_final())
			//	output << whitespace << keyword("final");

			if (!output.generate_definition() || c.is_declaration())
				output << /*punctuation(";") << */newl;
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
						need_sep = codegenImpl(generator, member, last_access);
					}
				}

				output.container_end();

				output.unindent();
				output << punctuation("};") << newl;
			}
		}
		return static_cast<bool>(output);
	}
}