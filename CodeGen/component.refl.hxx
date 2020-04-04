/*
 * @Author: your name
 * @Date: 2020-04-04 11:12:12
 * @LastEditTime: 2020-04-04 12:27:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\Coding\SakuraAutoCoder\CodeGen\component.refl.hxx
 */
#pragma once
#include <string>

namespace Sakura::Test
{
    struct [[component]] TestComponent
    {
        [[norefl]] float attrib;
        [[meta("Fxxk ISO C++23")]] std::string name;
        [[meta("Can't wait to use compile-time-reflection")]]
            void Method(std::string inVal, TestComponent& inRef);
        static float staticAttrib;
    };
}