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
        [[meta("This is a test name.")]] std::string name;
        void Method(std::string inVal, TestComponent& inRef);
        static float staticAttrib;
    };
}