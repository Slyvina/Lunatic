// Lic:
// Lunatic/Lunatic.hpp
// Lunatic (header)
// version: 23.01.04
// Copyright (C) 2022, 2023 Jeroen P. Broks
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

namespace Slyvina {

	namespace Lunatic {

		class _Lunatic;

		typedef std::shared_ptr<_Lunatic> Lunatic;
		typedef std::unique_ptr<_Lunatic> ULunatic;

		class _Lunatic {
		private:
			lua_State* _State{ nullptr };
			static std::map<std::string, lua_CFunction> RegFunction;
		public:
			static lua_CFunction Panick; 
			inline lua_State* State() { return _State; } // This makes the state not directly writeable from outside the class.
			inline static std::string Lua_Version() { return std::string(LUA_VERSION_MAJOR) + "." + std::string(LUA_VERSION_MINOR) + "." + std::string(LUA_VERSION_RELEASE); }
			
			void Register(std::string name, lua_CFunction func, bool force = false);
			inline void Register(std::map<std::string, lua_CFunction> Stuff) { for (auto& St : Stuff) Register(St.first, St.second); }

			void QDoString(std::string source);

			_Lunatic();
			~_Lunatic();

		};

		Lunatic LunaticBySource(std::string source);
		

	}

}