/*
 * @Author: your name
 * @Date: 2020-04-04 11:11:49
 * @LastEditTime: 2020-04-05 15:28:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Editi
 * @FilePath: \undefinedd:\Coding\SakuraAutoCoder\CodeGen\component.generated.hxx
 */
#pragma once
#include <refl.rule.hxx>
#include "component.refl.hxx"

namespace Sakura::Test
{
    using namespace Sakura::refl;
    template<>
    struct SClass<TestComponent> final : public IClass
    {
        inline static const constexpr char name[] = "TestComponent";
        inline static const constexpr std::size_t id_ = 
            _Fnv1a_append_bytes(_FNV_offset_basis, name, length(name) * sizeof(char));
        virtual const char* GetName() const override final;
        virtual Reference GetField(const Reference& o, const std::string& name) const override final;
    };
}