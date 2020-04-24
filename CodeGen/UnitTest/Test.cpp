/*
 * @Author: your name
 * @Date: 2020-04-22 00:49:46
 * @LastEditTime: 2020-04-24 18:34:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/UnitTest/Test.cpp
 */
#include <iostream>
#include "include/refl.rule.hxx"
#include "component.refl.hxx"

using namespace Sakura::refl;

int main(void)
{
	//printFieldMeta<TestComponentWrap>();
	Test::TestComponent testComp;

	Test::TestComponentWrap testCompWrap;
	SClass<std::decay<decltype(testComp)>::type>::ForEachField(testComp,
		Sakura::overload(
			[](auto&& field, const Field& meta) {
				std::cout
					<< meta.type << " - "
					<< meta.name << ": " << field << std::endl;
			}));

	std::cout << std::endl << std::endl;
	SClass<std::decay<decltype(testCompWrap)>::type>::ForEachFieldAtomic(testCompWrap,
		Sakura::overload(
			[](float field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			},
			[](const std::string& field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			},
			[](Test::TestComponent* field, const Field& meta) {
				std::cout << "TestCompPtr" << std::endl;
			}));

	std::cout << std::endl << std::endl;
	SClass<Test::TestComponentWrap>::ForEachStaticFieldAtomic(
		Sakura::overload(
			[](float field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			},
			[](const std::string& field, const Field& meta) {
				std::cout << meta.name << ": " << field << std::endl;
			}));
	
	SClass<std::decay<decltype(testComp)>::type>::ForEachMethod(testComp,
		[&](auto&& method, const Field& meta)
		{
			if constexpr
				(std::is_same<typename std::decay<decltype(method)>::type,
					 void(Test::TestComponent::*)(Test::TestComponent*)>())
			{
				std::cout << meta.name << ": " << meta.type << std::endl;
				(testComp.*method)(nullptr);
			}
		});
	return 0;
}
