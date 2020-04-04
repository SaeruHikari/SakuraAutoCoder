# Copyright (C) 2017-2018 Jonathan Müller <jonathanmueller.dev@gmail.com>
# This file is subject to the license terms in the LICENSE file
# found in the top-level directory of this distribution.

add_executable(cppast_tool main.cpp)
include_directories(../external/cxxopts/include/)
target_link_libraries(cppast_tool PUBLIC cppast)
set_target_properties(cppast_tool PROPERTIES CXX_STANDARD 11 OUTPUT_NAME cppast)
