#include "codegen.class.hpp"
#include <cppast/cpp_alias_template.hpp>
#include <cppast/cpp_entity_kind.hpp>
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
#include <iostream>

extern bool bDebugAST;
extern std::unordered_map<std::string, Sakura::refl::ReflUnit> ReflUnits;

namespace Sakura::refl
{
	namespace detail
	{
		auto get_class_entity_Name_sp(const cpp_class& c)
		{
			const cpp_entity* unit = &c;
			std::string _n = c.name();
			std::string _t = "::";
			while(unit->parent().has_value() &&
				(unit->parent().value().kind() == cpp_entity_kind::namespace_t ||
				 unit->parent().value().kind() == cpp_entity_kind::class_t))
			{
				_n = unit->parent().value().name() + _t + _n;
				unit = &unit->parent().value();
			}
			return _n;
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
		
		void gen_getClassName(code_generator::output& output, const cpp_class& c)
		{
			output << punctuation("\t");
			detail::inline_static_const_constexpr(output);
			output << keyword("const") << cppast::whitespace << cppast::identifier("std::string_view")
				<< cppast::whitespace << identifier("GetClassName") << punctuation("()") 
				<< cppast::newl << cppast::punctuation("\n\t{\n");
			output << cppast::punctuation("\t\t") << keyword("return") << cppast::whitespace
					<< cppast::token_seq("Sakura::refl::decay_type_name<")
					<< cppast::identifier(get_class_entity_Name_sp(c)) << cppast::token_seq(">()")
					<< cppast::punctuation(";\n\t}\n");
		}

		void gen_getPrettyName(code_generator::output& output, const cpp_class& c)
		{
			std::string name = "\"" + get_class_entity_Name_sp(c) + "\"";
			for(auto&& iter : c.attributes())
			{
				if(iter.name() == "prettyname")
					name = iter.arguments().value().as_string();
			}
			output << punctuation("\t");
			detail::inline_static_const_constexpr(output);
			output << keyword("const") << cppast::whitespace << cppast::identifier("std::string_view")
				<< cppast::whitespace << identifier("GetPrettyName") << punctuation("()") 
				<< cppast::newl << cppast::punctuation("\n\t{\n");
			output << cppast::punctuation("\t\t") << keyword("return") 
					<< cppast::whitespace << cppast::string_literal(name)
					<< cppast::punctuation(";\n\t}\n");
		}

		void collect_method_meta(ReflField& field, const cppast::cpp_entity& member)
		{
			field.name = member.name().c_str();
			field.type = "method";
			//field.type
			//	= cppast::to_string(((const cppast::cpp_member_function&)member).);
			for (auto i = 0; i < member.attributes().size(); i++)
			{
				std::pair<std::string, std::string> attrib{ member.attributes()[i].name() , "" };
				if (member.attributes()[i].arguments().has_value())
					attrib.second = member.attributes()[i].arguments().value().as_string();
				field.fieldMetas.insert(attrib);
			}
		}

		void collect_static_method_meta(ReflField& field, const cppast::cpp_entity& member)
		{
			field.name = member.name().c_str();
			field.type = "static_method";
			//	= cppast::to_string(((const cppast::cpp_member_function&)member).);
			for (auto i = 0; i < member.attributes().size(); i++)
			{
				std::pair<std::string, std::string> attrib{ member.attributes()[i].name() , "" };
				if (member.attributes()[i].arguments().has_value())
					attrib.second = member.attributes()[i].arguments().value().as_string();
				field.fieldMetas.insert(attrib);
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
			reflUnit.unitName = detail::get_class_entity_Name_sp(c);
			for (auto i = 0; i < c.attributes().size(); i++)
			{
				std::pair<std::string, std::string> attrib{ c.attributes()[i].name() , "" };
				if (c.attributes()[i].arguments().has_value())
					attrib.second = c.attributes()[i].arguments().value().as_string();
				reflUnit.unitMetas.insert(attrib);
			}
			if(reflUnit.unitMetas.find("atomic") != reflUnit.unitMetas.end())
				output << token_seq("template<> inline const constexpr bool Sakura::refl::isAtomic<")
				<< identifier(reflUnit.unitName)
				<< token_seq(">(){return true;}\n");
			if(reflUnit.unitMetas.find("refl") != reflUnit.unitMetas.end())
			{
				output << identifier("template<>\nstruct ClassInfo<" + 
					detail::get_class_entity_Name_sp(c) + ">\n{\n");
			}
			else // If class is not tagged with refl, return directly
				return true;
			detail::gen_getClassName(output, c);
			detail::gen_getPrettyName(output, c);
			detail::gen_meta(output, "", reflUnit.unitMetas);
			// Iterate
			for (auto& member : c)
			{
				if (member.kind() == cpp_entity_kind::member_variable_t)
				{
					detail::collect_field_meta(reflUnit.fieldsMap[member.name()], member);
					if(reflUnit.fieldsMap[member.name()].fieldMetas.find("refl") 
						!= reflUnit.fieldsMap[member.name()].fieldMetas.end())
					generate_meta_class_member_varable(output, ((const cpp_member_variable&)member),
						reflUnit.fieldsMap[member.name()]);
				}
				else if (member.kind() == cpp_entity_kind::variable_t)
				{
					detail::collect_static_field_meta(reflUnit.staticFieldsMap[member.name()], member);
					if(reflUnit.staticFieldsMap[member.name()].fieldMetas.find("refl") 
						!= reflUnit.staticFieldsMap[member.name()].fieldMetas.end())
					generate_meta_class_member_varable(output, ((const cpp_member_variable&)member),
						reflUnit.staticFieldsMap[member.name()]);
				}
				else if (member.kind() == cpp_entity_kind::member_function_t)
				{
					detail::collect_method_meta(reflUnit.methodsMap[member.name()], member);
					if(reflUnit.methodsMap[member.name()].fieldMetas.find("refl") 
						!= reflUnit.methodsMap[member.name()].fieldMetas.end())
					generate_meta_class_member_varable(output, ((const cpp_member_variable&)member),
						reflUnit.methodsMap[member.name()]);
				}
				else if (member.kind() == cpp_entity_kind::function_t)
				{
					detail::collect_static_method_meta(reflUnit.staticMethodsMap[member.name()], member);
					if(reflUnit.staticMethodsMap[member.name()].fieldMetas.find("refl") 
						!= reflUnit.staticMethodsMap[member.name()].fieldMetas.end())
					generate_meta_class_member_varable(output, ((const cpp_member_variable&)member),
						reflUnit.staticMethodsMap[member.name()]);
				}
			}
			detail::gen_all_fields(output, reflUnit, detail::FieldSet::E_FIELDS);
			detail::gen_all_fields(output, reflUnit, detail::FieldSet::E_STATIC_FIELDS);
			detail::gen_all_fields(output, reflUnit, detail::FieldSet::E_METHODS);
			detail::gen_all_fields(output, reflUnit, detail::FieldSet::E_STATIC_METHODS);
			output << punctuation("};\n\n");
		}
		ReflUnits[c.name()] = reflUnit;
		return static_cast<bool>(output);
	}
}