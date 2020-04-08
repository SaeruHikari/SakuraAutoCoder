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
#include <cppast/cpp_namespace.hpp>
#include <cppast/cpp_preprocessor.hpp>
#include <cppast/cpp_static_assert.hpp>
#include <cppast/cpp_template_parameter.hpp>
#include <cppast/cpp_token.hpp>
#include <cppast/cpp_type_alias.hpp>
#include <cppast/cpp_variable.hpp>
#include <iostream>

extern bool bDebugAST;
extern std::unordered_map<std::string, Sakura::refl::ReflUnit> ReflUnits;

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
		
		void gen_getClassName(code_generator::output& output, const cpp_class& c)
		{
			detail::inline_static_const_constexpr(output);
			output << keyword("const") << cppast::whitespace << cppast::identifier("char*")
				<< cppast::whitespace << identifier("GetClassName") << punctuation("()") << cppast::newl << cppast::punctuation("\n{\n");
			output << cppast::punctuation("    ") << keyword("return") << cppast::whitespace <<
				cppast::string_literal("\"") << cppast::string_literal(c.name()) 
					<< cppast::string_literal("\"") << cppast::punctuation(";\n}\n");
		}
		
		void gen_all_fields(code_generator::output& output, const ReflUnit& unit)
		{
			inline_static_const_constexpr(output);
			output << cppast::identifier("all_fields") << cppast::punctuation("()\n{\n");
			for (auto iter = unit.fieldsMap.begin(); iter != unit.fieldsMap.end(); iter++)
			{
				output << punctuation("    ") << cppast::identifier("SFIELD_INFO") << cppast::punctuation("(")
					<< identifier(iter->first) << punctuation(", ")
					<< identifier(iter->second.type) << punctuation(", ")
					<< identifier(unit.unitName) << punctuation(", ");
				if (iter->second.fieldMetas.size() > 0)
					output << cppast::identifier(iter->first + "_meta");
				else
					output << cppast::identifier("nullptr");
				output << punctuation(");\n");
			}
			output << punctuation("    ") << keyword("return") << punctuation(" ")
				<< identifier("hana::make_tuple") << punctuation("(");
			for (auto iter = unit.fieldsMap.begin(); iter != unit.fieldsMap.end(); iter++)
			{
				output << identifier(iter->first + "_info()");
				if(++iter != unit.fieldsMap.end())
					output << punctuation(", ");
				else
					output << punctuation(");\n}\n");
				iter--;
			}
		}
	}

	bool generate_class(
		std::function<bool(cppast::code_generator&,
			const cpp_entity&, cpp_access_specifier_kind)> codegenImpl,
		code_generator& generator, const cpp_class& c,
		cpp_access_specifier_kind cur_access,
		type_safe::optional_ref<const cpp_template_specialization> spec)
	{
		code_generator::output output(type_safe::ref(generator), type_safe::ref(c), cur_access);
		ReflUnit reflUnit;
		if (output)
		{
			using namespace cppast;
			// Pregen class
			reflUnit.unitName = c.name();
			for (auto i = 0; i < c.attributes().size(); i++)
			{
				std::pair<std::string, std::string> attrib{ c.attributes()[i].name() , "" };
				if (c.attributes()[i].arguments().has_value())
					attrib.second = c.attributes()[i].arguments().value().as_string();
				reflUnit.unitMetas.insert(attrib);
			}
			output << identifier(c.semantic_scope());
			if(reflUnit.unitMetas.find("component") != reflUnit.unitMetas.end())
			{
				output << identifier("template<>\nClassInfo<" + c.name() + ">\n{\n");
			}
			detail::gen_getClassName(output, c);
			detail::gen_meta(output, "", reflUnit.unitMetas);
			
			// Iterate
			for (auto& member : c)
			{
				if (member.kind() == cpp_entity_kind::member_variable_t)
				{
					reflUnit.fieldsMap[member.name()].name = member.name().c_str();
					cppast::detail::write_type(output, ((const cpp_member_variable&)member).type(), member.name());
					reflUnit.fieldsMap[member.name()].type 
						= cppast::to_string(((const cpp_member_variable&)member).type());
					for (auto i = 0; i < member.attributes().size(); i++)
					{
						std::pair<std::string, std::string> attrib{ member.attributes()[i].name() , "" };
						if (member.attributes()[i].arguments().has_value())
							attrib.second = member.attributes()[i].arguments().value().as_string();
						reflUnit.fieldsMap[member.name()].fieldMetas.insert(attrib);
					}
					generate_refl_class_member_varable(output, ((const cpp_member_variable&)member),
						reflUnit.fieldsMap[member.name()]);
				}
			}

			detail::gen_all_fields(output, reflUnit);
		}
		ReflUnits[c.name()] = reflUnit;
		return static_cast<bool>(output);
	}
}