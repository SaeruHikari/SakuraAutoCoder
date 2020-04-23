/*
 * @Author: your name
 * @Date: 2020-04-22 23:58:55
 * @LastEditTime: 2020-04-23 01:02:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/Common/TimeStrampHeader.hpp
 */
#pragma once
#include <fstream> 
#include <filesystem>
#include <ostream>

namespace Sakura::CommonGen
{
    void gen_timestamp_header(std::ostream& out,
        const std::filesystem::path& target)
    {
        std::time_t now = std::time(nullptr);
        out << "//generated time:" << now << "\n";
        out << "//generated end\n\n";
        return;
    }
}