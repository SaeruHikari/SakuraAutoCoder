/*
 * @Author: your name
 * @Date: 2020-04-23 14:18:01
 * @LastEditTime: 2020-04-24 01:56:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/include/sakura_cpp.parser.cpp
 */
#pragma once
#include <cppast/libclang_parser.hpp> // for libclang_parser, libclang_compile_config, cpp_entity,...

namespace Sakura::parser
{
    void sakura_cpp_config(cppast::libclang_compile_config& config, 
        const cxxopts::ParseResult& options) noexcept
    {
        if (options.count("database_dir"))
        {
            cppast::libclang_compilation_database database(
                options["database_dir"].as<std::string>());
            if (options.count("database_file"))
                config
                = cppast::libclang_compile_config(database,
                    options["database_file"].as<std::string>());
            else
                config
                = cppast::libclang_compile_config(database, options["file"].as<std::string>());
        }
        if (options.count("verbose"))
            config.write_preprocessed(true);

        if (options.count("fast_preprocessing"))
            config.fast_preprocessing(true);

        if (options.count("remove_comments_in_macro"))
            config.remove_comments_in_macro(true);

        if (options.count("include_directory"))
            for (auto& include : options["include_directory"].as<std::vector<std::string>>())
            {
                config.add_include_dir(include);
                std::cout << "Include: " << include << std::endl;
            }
        if (options.count("macro_definition"))
            for (auto& macro : options["macro_definition"].as<std::vector<std::string>>())
            {
                auto equal = macro.find('=');
                auto name = macro.substr(0, equal);
                if (equal == std::string::npos)
                    config.define_macro(std::move(name), "");
                else
                {
                    auto def = macro.substr(equal + 1u);
                    config.define_macro(std::move(name), std::move(def));
                }
            }
        if (options.count("macro_undefinition"))
            for (auto& name : options["macro_undefinition"].as<std::vector<std::string>>())
                config.undefine_macro(name);
        if (options.count("feature"))
            for (auto& name : options["feature"].as<std::vector<std::string>>())
                config.enable_feature(name);

        // the compile_flags are generic flags
        cppast::compile_flags flags;
        if (options.count("gnu_extensions"))
            flags |= cppast::compile_flag::gnu_extensions;
        if (options.count("msvc_extensions"))
            flags |= cppast::compile_flag::ms_extensions;
        if (options.count("msvc_compatibility"))
            flags |= cppast::compile_flag::ms_compatibility;
        config.set_flags(cppast::cpp_standard::cpp_1z, flags);
    }

    // parse a file
    std::unique_ptr<cppast::cpp_file> parse_file(const cppast::libclang_compile_config& config,
        const cppast::diagnostic_logger& logger,
        const std::string& filename, bool fatal_error) noexcept
    {
        // the entity index is used to resolve cross references in the AST
        // we don't need that, so it will not be needed afterwards
        cppast::cpp_entity_index idx;
        // the parser is used to parse the entity
        // there can be multiple parser implementations
        cppast::libclang_parser parser(type_safe::ref(logger));
        // parse the file
        auto file = parser.parse(idx, filename, config);
        // if (fatal_error && parser.error())
        //    return nullptr;
        return file;
    }
}