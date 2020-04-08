/*
 * @Author: your name
 * @Date: 2020-04-04 11:12:12
 * @LastEditTime: 2020-04-05 15:26:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\Coding\SakuraAutoCoder\CodeGen\component.refl.hxx
 */
#pragma once
#include <string>

namespace Sakura::Test
{
	struct [[component]] [[descriptions("This is a test component")]] TestComponent
	{
		[[meta("SaeruHikari")]]
		float attrib = 123.f;

		[[meta("Fxxk ISO C++23")]]
		std::string name = "TestComp";

		inline static const constexpr float staticAttrib = 155;

		[[meta("Can't wait to use compile-time-reflection")]]
		void Method(std::string inVal, TestComponent& inRef) {};
	};

	struct [[component]] TestComponentWrap
	{
		[[description("Test wrapping of reflection component")]]
		TestComponent comp;
		inline static const TestComponent statComp = { 14221.f, "Stat" };
		float wtf = 155.f;
	};
}