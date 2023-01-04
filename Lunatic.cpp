// Lic:
// Lunatic/Lunatic.cpp
// Lunatic
// version: 23.01.04
// Copyright (C) 2023 Jeroen P. Broks
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
// EndLic

#include <iostream>
#include "Lunatic.hpp"

namespace Slyvina {
	namespace Lunatic {

		lua_CFunction _Lunatic::Panick{ nullptr };
		std::map<std::string, lua_CFunction> _Lunatic::RegFunction{};

		void _Lunatic::Register(std::string name, lua_CFunction func, bool force) {
			if (RegFunction.count(name)) if (!force) { std::cout << "Dupe function register: " << name << "\tRequest ignored!\n"; return; }
			RegFunction[name] = func;
		}

		void _Lunatic::QDoString(std::string source) {
			if (!_State) { std::cout << "Lunatic: Operation cancelled! State is NULL\n"; return; }
			luaL_loadstring(_State, source.c_str());
			lua_call(_State, 0, 0);
		}

		_Lunatic::_Lunatic() {
			auto L{ luaL_newstate() };
			_State = L;
			if (Panick) lua_atpanic(L, Panick);
			luaL_openlibs(L);
		}

		_Lunatic::~_Lunatic() {
			if (_State) lua_close(_State);
			_State = nullptr;
		}
		
		Lunatic LunaticBySource(std::string source) {			
			auto ret{ std::make_shared<_Lunatic>() };
			ret->QDoString(source);
			return ret;
		}
	}

}