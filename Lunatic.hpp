// License:
// 	Lunatic/Lunatic.hpp
// 	Lunatic (header)
// 	version: 24.10.15
// 
// 	Copyright (C) 2022, 2023, 2024 Jeroen P. Broks
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
// Lic:
// Lunatic/Lunatic.hpp
// Lunatic (header)
// version: 24.10.08
// Copyright (C) 2022, 2023, 2024 Jeroen P. Broks
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

#pragma once

#pragma region Lua
extern "C" {
#include "Lua/Raw/src/lua.h"
#include "Lua/Raw/src/lualib.h"
#include "Lua/Raw/src/lauxlib.h"
}
#pragma endregion

#pragma region "C++ includes"
#include <memory>
#include <string>
#include <map>
#include <string>
#pragma endregion

#pragma region "Slyvina includes"
#include <SlyvBank.hpp>
#pragma endregion

namespace Slyvina {

	namespace NSLunatic {

		class _Lunatic;

		typedef std::shared_ptr<_Lunatic> Lunatic; 
		typedef std::shared_ptr<_Lunatic> SLunatic;
		typedef std::unique_ptr<_Lunatic> ULunatic;

		typedef std::map<std::string, lua_CFunction> LunaticRegMap;

		struct _LunDbg {
			std::string File{ "" };
			std::string Func{ "" };
			size_t Line{ 0 };
		};



		class _Lunatic {
		private:
			static uint64 count;
			uint64 ID;
			lua_State* _State{ nullptr };
			static std::map<std::string, lua_CFunction> RegFunction;
			std::map < std::string, bool> _Flags{};
			bool Released{ false };
		public:
			bool Debug{ false };
			std::vector<_LunDbg> Trace{};

			static lua_CFunction Panick; 
			inline lua_State* State() { return _State; } // This makes the state not directly writeable from outside the class.
			inline static std::string Lua_Version() { return std::string(LUA_VERSION_MAJOR) + "." + std::string(LUA_VERSION_MINOR) + "." + std::string(LUA_VERSION_RELEASE); }
			

			static void Register4All(std::string name, lua_CFunction func, bool force = false);
			static inline void Register4All(std::map<std::string, lua_CFunction> Stuff) { for (auto& St : Stuff) Register4All(St.first, St.second); }

			void Register(std::string FuncName, lua_CFunction FuncFunc);
			void Register(std::map<std::string, lua_CFunction> Stuff);

			void QDoString(std::string source);
			void QDoByteCode(char* buf, size_t len,std::string chunk="");
			void QDoByteCode(Units::Bank buf, std::string chunk = "");

			bool Flag(std::string fl);
			void Flag(std::string fl, bool v);

			/// <summary>
			/// Kills the Lua state within. Please note, this renders this object unusable. Normally this happens automatically when the Lunatic object is destroyed.
			/// </summary>
			void Kill(); 

			_Lunatic();
			~_Lunatic();

		};

		/// <summary>
		/// Creates a Lunatic state by parsing a Lua script from a string
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		Lunatic LunaticBySource(std::string source);

		/// <summary>
		/// Creates a Lunatic state by using Lua compiled code from a char* buffer
		/// </summary>
		/// <param name="buf"></param>
		/// <param name="len"></param>
		/// <param name="chunk"></param>
		/// <returns></returns>
		Lunatic LunaticByByteCode(char* buf,size_t len,std::string chunk="");

		/// <summary>
		/// Creates a Lunatic state by using Lua compiled code from a Slyvina Bank
		/// </summary>
		/// <param name="buf"></param>
		/// <param name="len"></param>
		/// <param name="chunk"></param>
		/// <returns></returns>
		Lunatic LunaticByByteCode(Units::Bank buf, std::string chunk = "");
		
		/// <summary>
		/// Quick and dirty function which will return a C++ string from a luaL_checkstring in stead of a const char*.
		/// </summary>
		/// <param name="L">The Lua State</param>
		/// <param name="pos">Position</param>
		/// <returns>The wanted string</returns>
		inline std::string Lunatic_CheckString(lua_State* L, int pos) { return luaL_checkstring(L, pos); }
		
		/// <summary>
		/// Quick and dirty function which will return a C++ string from a luaL_optstring in stead of a const char*.
		/// </summary>
		/// <param name="L">The Lua State</param>
		/// <param name="pos">Position</param>
		/// <param name="defaultvalue">The default value if the script didn't give a string</param>
		/// <returns>The wanted string</returns>
		inline std::string Lunatic_OptString(lua_State* L, int pos, std::string defaultvalue) { return luaL_optstring(L, pos, defaultvalue.c_str()); }

		/// <summary>
		/// Quick and dirty function that will allow C++ strings to be pushed to a lua state.
		/// </summary>
		/// <param name="L">State</param>
		/// <param name="str">The string</param>
		inline void Lunatic_PushString(lua_State* L, std::string str) { lua_pushstring(L, str.c_str()); }
		

	}

}
