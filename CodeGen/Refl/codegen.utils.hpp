#pragma once
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_file.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_template.hpp>
#include "include/refl.rule.hxx"

namespace Sakura::refl
{
	using cpp_entity = cppast::cpp_entity;
	using cpp_entity_kind = cppast::cpp_entity_kind;
	using keyword = cppast::keyword;
	using identifier = cppast::identifier;
	using punctuation = cppast::punctuation;
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
	using cpp_member_variable = cppast::cpp_member_variable;

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

		inline void inline_static_const_constexpr(code_generator::output& output)
		{
			output << keyword("inline") << cppast::whitespace << keyword("static")
				<< cppast::whitespace << keyword("const")
				<< cppast::whitespace << keyword("constexpr") << cppast::whitespace;
		}

		enum FieldSet
		{
			E_FIELDS,
			E_STATIC_FIELDS,
			E_METHODS,
			E_STATIC_METHODS,
			E_ENUM_VALUES
		};

		inline std::string GetMetaGetterName(FieldSet set)
		{
			switch (set)
			{
			case Sakura::refl::detail::E_FIELDS:
				return "all_fields";
			case Sakura::refl::detail::E_ENUM_VALUES:
			case Sakura::refl::detail::E_STATIC_FIELDS:
				return "all_static_fields";
			case Sakura::refl::detail::E_METHODS:
				return "all_methods";
			case Sakura::refl::detail::E_STATIC_METHODS:
				return "all_static_methods";
			default:
				break;
			}
		}

		inline std::string GetMetaMacroGenName(FieldSet set)
		{
			switch (set)
			{
			case Sakura::refl::detail::E_FIELDS:
				return "SFIELD_INFO";
			case Sakura::refl::detail::E_STATIC_FIELDS:
				return "SSTATICFIELD_INFO";
			case Sakura::refl::detail::E_STATIC_METHODS:
			case Sakura::refl::detail::E_METHODS:
				return "SMETHOD_INFO";
			case Sakura::refl::detail::E_ENUM_VALUES:
				return "SENUM_FIELD_INFO";
			default:
				break;
			}
		}

		inline auto& GetFieldMap(const ReflUnit& unit, FieldSet set)
		{
			switch (set)
			{
			case Sakura::refl::detail::E_FIELDS:
				return unit.fieldsMap;
			case Sakura::refl::detail::E_ENUM_VALUES:
			case Sakura::refl::detail::E_STATIC_FIELDS:
				return unit.staticFieldsMap;
			case Sakura::refl::detail::E_STATIC_METHODS:
				return unit.staticMethodsMap;
			case Sakura::refl::detail::E_METHODS:
				return unit.methodsMap;
			default:
				break;
			}
		}

		inline void gen_all_fields(code_generator::output& output, const ReflUnit& unit,
			FieldSet fieldSet)
		{
			auto fields = GetFieldMap(unit, fieldSet);
			if (fields.size() <= 0)
				return;
			int num = 0;
			for (auto iter = fields.begin(); iter != fields.end(); iter++)
			{
				if(iter->second.fieldMetas.find("refl")
					!= iter->second.fieldMetas.end() |
					fieldSet == FieldSet::E_ENUM_VALUES)
				{
					output << punctuation("\t") 
						<< cppast::identifier(GetMetaMacroGenName(fieldSet)) << cppast::punctuation("(")
						<< identifier(iter->first) << punctuation(", ")
						//<< identifier(iter->second.type) << punctuation(", ")
						<< identifier(unit.unitName) << punctuation(", ");
					if (iter->second.fieldMetas.size() > 0)
						output << cppast::identifier(iter->first + "_meta");
					else
						output << cppast::identifier("nullptr");
					output << punctuation(");\n");
					num++;
				}
			}			
			if(num <= 0)
				return;
			output << punctuation("\t");
			inline_static_const_constexpr(output);
			output << cppast::keyword("auto") << cppast::punctuation(" ")
				<< cppast::identifier(GetMetaGetterName(fieldSet)) << cppast::punctuation("()\n\t{\n");
			output << punctuation("\t\t") << keyword("return") << punctuation(" ")
				<< identifier("Sakura::make_tuple") << punctuation("(");
			auto i = 0u;
			for (auto iter = fields.begin(); iter != fields.end(); iter++)
			{
				if(iter->second.fieldMetas.find("refl")
					!= iter->second.fieldMetas.end()|
					fieldSet == FieldSet::E_ENUM_VALUES)
				{
					i++;
					output << identifier(iter->first + "_info()");
					if(i != num)
						output << punctuation(", ");
					else
						output << punctuation(");\n\t}\n");
				}
			}
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