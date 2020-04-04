/*
 * @Author: your name
 * @Date: 2020-04-04 11:11:49
 * @LastEditTime: 2020-04-04 12:47:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Editi
 * @FilePath: \undefinedd:\Coding\SakuraAutoCoder\CodeGen\component.generated.hxx
 */
#pragma once
#include "component.refl.hxx"

template<typename T>
class SClass
{
    template<typename... Args>
    delctype(auto) Invoke(const std::string& name, Args... args)
    {
        
    }
};

template<>
class SClass<std::void_t>
{

};

namespace Sakura::Test
{
    template<>
    struct SClass<TestComponent>
    {
         
    };

    struct TestComponent_ReflHelper
    {

    };
}