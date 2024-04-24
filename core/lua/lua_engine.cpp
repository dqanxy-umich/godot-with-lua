#include "lua_engine.h"


void LuaEngine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("hello_world"), &LuaEngine::hello_world);
	ClassDB::bind_method(D_METHOD("do_string", "code"), &LuaEngine::do_string);
}

LuaEngine::LuaEngine(){
	state = luaL_newstate();
	luaL_openlibs(state);
	//C Modules
	luaL_requiref(state, "pb", luaopen_pb, 1);
	luaL_requiref(state, "pb.io", luaopen_pb_io, 1);
	luaL_requiref(state, "pb.conv", luaopen_pb_conv, 1);
	luaL_requiref(state, "pb.buffer", luaopen_pb_buffer, 1);
	luaL_requiref(state, "pb.slice", luaopen_pb_slice, 1);
	luaL_requiref(state, "pb.unsafe", luaopen_pb_unsafe, 1);

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