/*
 * @Author: your name
 * @Date: 2020-04-22 00:49:46
 * @LastEditTime: 2020-04-27 16:37:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /SakuraAutoCoder/CodeGen/UnitTest/Test.cpp
 */
#include <iostream>
#include "include/refl.rule.hxx"
#include "component.refl.hxx"
#include "include/constexpr_map.hpp"

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
has_member(attrib);

template<typename T>
void auto_func_template(T&& field, const Field& meta,
	const std::string_view prefix)
{
	using TT = std::decay_t<decltype(field)>;
	if constexpr (Sakura::refl::isAtomic<TT>())
		std::cout << prefix << ": "
			 << meta.type << " - " << meta.name << std::endl;
	else if constexpr (
		Sakura::refl::has_member_all_fields<ClassInfo<TT>>::value)
	{
		std::cout << "FOREACH OF CHILD FIELD:\n";
		SClass<TT>::ForEachField(field, 
			[&](auto&& _field, auto&& _meta){
				auto_func_template(_field, _meta.fd, meta.name);
			});
		std::cout << "END FOREACH OF CHILD FIELD\n";
	}
}


struct Color
{
	float R;
	float G;
	float B;
	float A;
};
constexpr static auto const colors =
	Sakura::unordered_map_c<std::string_view, Color>({
		{ "red"sv, { 255, 0, 0, 1 } },
		{ "green"sv, { 0, 128, 0, 1 } },
		{ "yellow"sv, { 255, 255, 0, 1 } },
		{ "white"sv, { 255, 255, 255, 1 } },
		{ "black"sv, { 0, 0, 0, 1 } }
	});

int main(void)
{
	constexpr bool value = has_member_attrib<Test::TestComponent>::value;
	constexpr auto c = colors.find("red"sv)->second;
	//std::cout << call([](int, int, int, int){return 5;}, arr);
	Test::TestComponent testComp;
	Test::TestComponentWrap testCompWrap;
	SClass<std::decay<decltype(testComp)>::type>::ForEachField(testComp,
		[](auto&& field, auto&& meta) {
			std::cout << meta.fd.type << " - " << meta.fd.name << std::endl;
		});

	std::cout << std::endl << std::endl;
	
	// lambda: not so conveniet to call	recursively...use tempalte
	/*
	decltype(auto) accessor = 
		[&](auto&& self, auto&& field, const Field& meta) {
			using fieldT = std::decay_t<decltype(field)>;
			auto functor  =
				[&](auto&& field, const Field& meta){
					self(self, field, meta);
				};
			if constexpr (Sakura::refl::isAtomic<fieldT>())
				std::cout << meta.type << " - " << meta.name << std::endl;
			else if constexpr (Sakura::refl::has_member_all_fields<ClassInfo<fieldT>>::value)
			{
				std::cout << "FOREACH OF CHILD FIELD:\n";
				SClass<fieldT>::ForEachField(field, functor);
				std::cout << "END FOREACH OF CHILD FIELD\n";
			}
		};
	auto functor =
		[&](auto&& field, const Field& meta){
			accessor(accessor, field, meta);
		};*/

	SClass<std::decay<decltype(testCompWrap)>::type>::ForEachField(
		testCompWrap, [](auto&& field, auto&& meta){
			auto_func_template(field, meta.fd, "");
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
