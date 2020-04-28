/*
 * @Author: your name
 * @Date: 2020-04-22 00:49:46
 * @LastEditTime: 2020-04-28 21:54:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/UnitTest/Test.cpp
 */
#include <iostream>
#include "include/refl.rule.hxx"
#include "component.refl.hxx"
#include "include/constexpr_map.hpp"

using namespace Sakura::Refl;

has_member(attrib);

template<typename T>
void auto_func_template(T&& field, const Field& meta,
	const std::string_view prefix)
{
	using TT = std::decay_t<decltype(field)>;
	if constexpr (Sakura::Refl::isAtomic<TT>())
		std::cout << prefix << ": "
			 << meta.type << " - " << meta.name << std::endl;
	else if constexpr (
		Sakura::Refl::has_member_all_fields<ClassInfo<TT>>::value)
	{
		std::cout << "FOREACH OF CHILD FIELD:\n";
		SClass<TT>::ForEachField(field, 
			[&](auto&& _field, auto&& _meta){
				auto_func_template(_field, _meta, meta.name);
			});
		std::cout << "END FOREACH OF CHILD FIELD\n";
	}
}

constexpr bool sv_same(std::string_view s, std::string_view t)
{
    return s == t;
}

constexpr static const Sakura::detail::map_c<
	Sakura::detail::element<std::string_view, std::string_view>, 0> 
	dsd(nullptr);
	



int main(void)
{
	Test::TestComponent testComp;
	Sakura::Archive::SerializeToXml(testComp, "TestComponent.xml"sv);
	testComp.attrib = 12332;
	testComp = Sakura::Archive::DesrializeFromXml<Test::TestComponent>("TestComponent.xml"sv);
	std::cout << testComp.attrib << std::endl;
	Test::TestComponentWrap testCompWrap;
	SClass<std::decay<decltype(testComp)>::type>::ForEachField(
        testComp, [](auto&& field, auto&& info){
            constexpr const std::string_view tag = "container";
            
			if (info.metas.find("container") != info.metas.end())
				std::cout << info.metas.find("container")->second << std::endl;
    });

	SClass<std::decay<decltype(testCompWrap)>::type>::ForEachField(
		testCompWrap, [](auto&& field, auto&& meta){
			auto_func_template(field, meta, "");
		});

	std::cout << std::endl << std::endl;
	SClass<Test::TestComponentWrap>::ForEachStaticField(
			[](auto&& field, auto&& meta){
				
			});
	
	SClass<std::decay<decltype(testComp)>::type>::ForEachMethod(testComp,
		[&](auto&& method, auto&& meta)
		{
			if constexpr
				(std::is_same<typename std::decay<decltype(method)>::type,
					 void(Test::TestComponent::*)(Test::TestComponent*)>())
			{
				std::cout << meta.name << ": " << meta.type << std::endl;
				(testComp.*method)(nullptr);
			}
		});

	SEnum<Test::TestEnum>::ForEachStaticField(
		[&](Test::TestEnum val, auto&& meta)
		{
			std::cout << (std::size_t)val << " " << meta.name << std::endl;
		});
	//std::cout << SEnum<Test::TestEnum>::ToString(Test::TestEnum::E_ONE);
	return 0;
}
