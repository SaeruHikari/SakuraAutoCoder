/*
 * @Author: your name
 * @Date: 2020-04-23 00:08:32
 * @LastEditTime: 2020-04-25 02:13:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/include/filesystem.utils.h
 */
#pragma once
#include <iostream>
#include <fstream> 
#include <filesystem>
#include <unordered_map>

namespace Sakura::fs
{
    auto& parse_header_info(const std::filesystem::path& genFile)
    {
        static std::unordered_map<std::string, std::string> result;
        std::ifstream outf(genFile.c_str(), std::ios::binary);
        std::string line;
        std::string prefix = "//generated";
        while(std::getline(outf, line))
        {
            if(strncmp(line.c_str(), prefix.c_str() , prefix.length()) == 0)
            {
                auto seq = line.substr(prefix.size(), line.size());
                auto cut = seq.find(":");
                if(cut > 0)
                    result[seq.substr(1, cut - 1)] = seq.substr(cut + 1, seq.size());
            }
            else return result;
        }
    }
    bool generated_file_valid(const std::filesystem::path& src,
        const std::filesystem::path& genFile, bool debugInfo = false)
    {
        namespace fs = std::filesystem;
        auto ftime = fs::last_write_time(src);
        if (!fs::exists(genFile))
            return false;
        auto gentime = fs::last_write_time(genFile);
        std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
        std::time_t cgentime = decltype(gentime)::clock::to_time_t(gentime);
        std::time_t timebias = cgentime - cftime;
        if(debugInfo)
    		std::cout << "Time bias is " << timebias << '\n';
        if(timebias > 0)
        {
            auto varMap = parse_header_info(genFile);
            auto realMTime = std::atoi(varMap["time"].c_str());
            if(realMTime < cgentime)
            {
                if(true)
                    std::cout << "Generated file editted unexpectly, would not re-generate one!" << std::endl;
                return false;
            }
            else return true;
        }
        return false;
    }
}