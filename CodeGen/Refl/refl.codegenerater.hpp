/*
 * @Author: your name
 * @Date: 2020-04-23 14:35:07
 * @LastEditTime: 2020-04-23 14:44:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Refl/refl.codegenerater.hpp
 */
#pragma once
#include <cppast/cpp_entity_kind.hpp>        // for the cpp_entity_kind definition
#include <cppast/code_generator.hpp>         // for generate_code()
#include "codegen.hpp"

namespace Sakura::refl
{
    // prints the AST entry of a cpp_entity (base class for all entities),
    // will only print a single line
    void print_entity(std::ostream& out, const cppast::cpp_entity& e)
    {
        // print number of attributes
        if (e.attributes().empty())
        {
            return;
        }
        else 
        {
            if (e.kind() == cppast::cpp_entity_kind::language_linkage_t)
                // no need to print additional information for language linkages
                out << '\n';
            else if (e.kind() == cppast::cpp_entity_kind::namespace_t)
            {
                // cast to cpp_namespace
                auto& ns = static_cast<const cppast::cpp_namespace&>(e);
                // print whether or not it is inline
                out << '\n';
            }
            else
            {
                // print the declaration of the entity
                // it will only use a single line
                // derive from code_generator and implement various callbacks for printing
                // it will print into a std::string
                class code_generator : public cppast::code_generator
                {
                    std::string str_;                 // the result
                    bool        was_newline_ = false; // whether or not the last token was a newline
                    // needed for lazily printing them

                public:
                    code_generator(const cppast::cpp_entity& e)
                    {
                        // kickoff code generation here
                        cppast::generate_code_custom(*this, e, Sakura::refl::generate_code_impl);
                    }

                    // return the result
                    const std::string& str() const noexcept
                    {
                        return str_;
                    }

                private:
                    // called to retrieve the generation options of an entity
                    generation_options do_get_options(const cppast::cpp_entity&,
                        cppast::cpp_access_specifier_kind) override
                    {
                        // generate declaration only
                        return code_generator::declaration;
                    }

                    // no need to handle indentation, as only a single line is used
                    void do_indent() override {}
                    void do_unindent() override {}

                    // called when a generic token sequence should be generated
                    // there are specialized callbacks for various token kinds,
                    // to e.g. implement syntax highlighting
                    void do_write_token_seq(cppast::string_view tokens) override
                    {
                        if (was_newline_)
                        {
                            // lazily append newline as space
                            str_ += ' ';
                            was_newline_ = false;
                        }
                        // append tokens
                        str_ += tokens.c_str();
                    }

                    // called when a newline should be generated
                    // we're lazy as it will always generate a trailing newline,
                    // we don't want
                    void do_write_newline() override
                    {
                        was_newline_ = true;
                    }

                } generator(e);
                // print generated code
                out << generator.str();
            }
        }
    }
    
    // prints the AST of a file
    void gen_refl_code(std::ostream& out, const cppast::cpp_file& file)
    {
        // print file name
        std::string prefix; // the current prefix string
        out << "\n#pragma once\n";
        out << "#include <refl.rule.hxx>\n\n";
        out << "using namespace Sakura::refl;\n\n";
        // recursively visit file and all children
        cppast::visit(file, [&](const cppast::cpp_entity& e, cppast::visitor_info info)
        {
            if (e.kind() == cppast::cpp_entity_kind::file_t || cppast::is_templated(e)
                || cppast::is_friended(e))
                // no need to do anything for a file,
                // templated and friended entities are just proxies, so skip those as well
                // return true to continue visit for children
                return true;
            else if (info.event == cppast::visitor_info::container_entity_exit)
            {
                // Do nothing when exiting an entity containter.
            }
            else
            {
                print_entity(out, e);
            }
            return true;
        });
    }
}