#include "lua_engine.h"

void LuaEngine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("hello_world"), &LuaEngine::hello_world);
	ClassDB::bind_method(D_METHOD("do_string", "code"), &LuaEngine::do_string);
}

LuaEngine::LuaEngine(){
	state = luaL_newstate();
	luaL_openlibs(state);
}
LuaEngine::~LuaEngine(){
	lua_close(state);
}

void LuaEngine::hello_world(){
    
	ERR_PRINT("Hello World!");
}

String LuaEngine::do_string(String code){
	if(luaL_dostring(state, code.utf8().get_data()) == LUA_OK){
		if(lua_gettop(state)>0){
			auto result = lua_tostring(state, lua_gettop(state));
			lua_pop(state, lua_gettop(state));
			return String(result);
		}
		else{
			return String("");
		}
	}
	else{
		auto error = lua_tostring(state, lua_gettop(state));
       	ERR_PRINT(error);
        lua_pop(state, lua_gettop(state));
		return String(error);
	}
}