// License:
// 	Lunatic/Lunatic.cpp
// 	Lunatic
// 	version: 24.11.16
// 
// 	Copyright (C) 2023, 2024 Jeroen P. Broks
// 
// 	This software is provided 'as-is', without any express or implied
// 	warranty.  In no event will the authors be held liable for any damages
// 	arising from the use of this software.
// 
// 	Permission is granted to anyone to use this software for any purpose,
// 	including commercial applications, and to alter it and redistribute it
// 	freely, subject to the following restrictions:
// 
// 	1. The origin of this software must not be misrepresented; you must not
// 	   claim that you wrote the original software. If you use this software
// 	   in a product, an acknowledgment in the product documentation would be
// 	   appreciated but is not required.
// 	2. Altered source versions must be plainly marked as such, and must not be
// 	   misrepresented as being the original software.
// 	3. This notice may not be removed or altered from any source distribution.
// End License

#undef Lunatic_Debug

#include <iostream>
#include <SlyvString.hpp>
#include "Lunatic.hpp"

using namespace Slyvina::Units;

namespace Slyvina {
	namespace NSLunatic {

		lua_CFunction _Lunatic::Panick{ nullptr };
		std::map<std::string, lua_CFunction> _Lunatic::RegFunction{};
		uint64 _Lunatic::count{ 0 };

		void _Lunatic::Register4All(std::string name, lua_CFunction func, bool force) {
			if (RegFunction.count(name)) if (!force) { std::cout << "Dupe function register: " << name << "\tRequest ignored!\n"; return; }
			RegFunction[name] = func;
		}

		void _Lunatic::Register(std::string FuncName, lua_CFunction FuncFunc) {
			lua_register(_State, FuncName.c_str(), FuncFunc);
		}

		void _Lunatic::Register(std::map<std::string, lua_CFunction> Stuff) {
			for (auto Reg : Stuff) Register(Reg.first, Reg.second);
		}

		void _Lunatic::QDoString(std::string source) {
			if (!_State) { std::cout << "Lunatic: Operation cancelled! State is NULL\n"; return; }
			luaL_loadstring(_State, source.c_str());
			lua_call(_State, 0, 0);
		}

		void _Lunatic::QDoByteCode(char* buf, size_t len, std::string chunk) {
			if (!_State) { std::cout << "Lunatic: Operation cancelled! State is NULL\n"; return; }
			if (!chunk.size()) chunk = "<Nameless Chunk>";
			luaL_loadbuffer(_State, buf, len, chunk.c_str());
			lua_call(_State, 0, 0);
		}

		void _Lunatic::QDoByteCode(Units::Bank buf, std::string chunk) {
			if (buf->Expandable()) throw std::runtime_error("QByteCode can NOT be used with expandable banks!");
			QDoByteCode(buf->Direct(), buf->Size(), chunk);
		}

		bool _Lunatic::Flag(std::string fl) {
			Trans2Upper(fl);
			if (!_Flags.count(fl))	return false;
			return _Flags[fl];
		}

		void _Lunatic::Flag(std::string fl, bool v) {
			Trans2Upper(fl);
			_Flags[fl] = v;
		}


		_Lunatic::_Lunatic() {
			auto L{ luaL_newstate() };
			_State = L;
			if (Panick) lua_atpanic(L, Panick);
			luaL_openlibs(L);
			Register(RegFunction);
			ID = count++;
#ifdef Lunatic_Debug
			std::cout << ID << ":Created Lunatic state: " << (unsigned long long)_State << "/" << (uint64)this << "\n";
#endif
		}

		_Lunatic::~_Lunatic() {
			Kill();
		}
		void _Lunatic::Kill() {		
#ifdef Lunatic_Debug
			std::cout << ID << ":Releasing(" << Released << ") Lunatic state : " << (unsigned long long)_State << " / " << (uint64)this << "\n";
#endif
			if (_State && (!Released)) { lua_close(_State); Released = true; }
			_State = nullptr;
			Released = true;
		}

		Lunatic LunaticBySource(std::string source) {
			auto ret{ std::make_shared<_Lunatic>() };
			ret->QDoString(source);
			return ret;
		}

		Lunatic LunaticByByteCode(char* buf, size_t len, std::string chunk) {
			auto ret{ std::make_shared<_Lunatic>() };
			ret->QDoByteCode(buf, len, chunk);
			return ret;
		}

		Lunatic LunaticByByteCode(Slyvina::Units::Bank buf, std::string chunk) {
			return LunaticByByteCode(buf->Direct(), buf->Size(), chunk);
		}
		bool Lunatic_CheckBoolean(lua_State* L, int pos,bool AnythingGoes) {
			auto top = lua_gettop(L); 
			if (pos > top) {
				luaL_argerror(L, pos, TrSPrintF("Boolean value expected as parameter #%d, but only %d parameters were given", pos, top).c_str()); return false;
			}
			if (lua_type(L, pos) != LUA_TBOOLEAN) {
				if (AnythingGoes) {
					switch (lua_type(L, pos)) {
						case LUA_TNUMBER: return luaL_checknumber(L, pos) != 0; 
						case LUA_TSTRING: return Lunatic_CheckString(L, pos).size() != 0;
						case LUA_TNIL: return false;
						default: return true;
					}
				}
				luaL_argerror(L, pos, TrSPrintF("Boolean value expected for paramter #%d", pos).c_str());
				return false;
			}
			return lua_toboolean(L,pos);
		}
	}

}
