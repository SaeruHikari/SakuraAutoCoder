/*
 * @Author: your name
 * @Date: 2020-04-04 11:12:12
 * @LastEditTime: 2020-04-23 02:01:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\Coding\SakuraAutoCoder\CodeGen\component.refl.hxx
 */
#pragma once
#include <string> 
  
   
struct [[refl]] [[descriptions("This is a test component")]] TestComponent
{ 
	[[meta("SaeruHikari")]] 
	float attrib = 123.f; 

	[[meta("Fxxk ISO C++23")]] 
	std::string name = "TestComp";
 
	[[meta("This is a static attribute")]]
	inline static const constexpr float staticAttrib = 155;  

	[[meta("Can't wait to use compile-time-reflection")]]
	void Method(std::string inVal, TestComponent& inRef) {}; 
};    
  
struct [[refl]] TestComponentWrap
{ 
	TestComponent comp [[description("Test wrapping of reflection component")]];
	inline static const TestComponent statComp = { 14221.f, "Stat" };
	float wtf = 155.f; 
}; 

enum class [[refl]] TestEnum : uint8_t   
{   
	E_ONE [[ref]],  
	E_TWO,   
	E_THREE   
};    
   
#include "component.gen.hxx"   