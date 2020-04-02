cmake_minimum_required (VERSION 2.8.7)
project (SReflCodeGen)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}) 
 
set(CMAKE_C_STANDARD 11) 
set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -stdlib=libc++")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
elseif(MSVC)
	set(LLVM_CONFIG true)
	set(CLANG_INCLUDEDIR "D:/ToolSet/LLVM/llvm/tools/clang/include")
	set(CLANG_LIBDIR "D:/ToolSet/LLVM/llvm/tools/clang/lib")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(LibClang REQUIRED)
include_directories(${CLANG_INCLUDEDIR}) 

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#add_library(SReflRuntime STATIC lib/reflang.hpp lib/reflang.cpp)

add_executable(
	SReflCodeGen
	SReflCodeGen.cpp
	CodeGen/Refl/types.hpp
	CodeGen/Refl/types.cpp
	CodeGen/Refl/parser.cpp)
target_link_libraries(SReflCodeGen ${CLANG_LIBS})
