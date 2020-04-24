/*
 * @Author: your name
 * @Date: 2020-04-22 00:49:46
 * @LastEditTime: 2020-04-24 22:47:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/UnitTest/Test.cpp
 */
#include <iostream>
#include "include/refl.rule.hxx"
#include "component.refl.hxx"

using namespace Sakura::refl;

template <typename F, typename T, std::size_t N, std::size_t... Idx>
decltype(auto) call_impl(F f, T(&t)[N], std::index_sequence<Idx...>) 
{
    return f(t[Idx]...);
}
template <typename F, typename T, std::size_t N>
decltype(auto) call(F f, T (&t)[N]) 
{
    return call_impl(f, t, std::make_index_sequence<N>{});   
}
int arr[4] = {1, 2, 3, 4};

int main(void)
{
	std::cout << call([](int, int, int, int){return 5;}, arr);
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

	SEnum<Test::TestEnum>::ForEachStaticField(
		[&](Test::TestEnum val, const Field& meta)
		{
			std::cout << (std::size_t)val << " " << meta.name << std::endl;
		});
	std::cout << SEnum<Test::TestEnum>::ToString(Test::TestEnum::E_ONE);
	return 0;
}
