#include "lua_engine.h"

void LuaEngine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("hello_world"), &LuaEngine::hello_world);
}

void LuaEngine::hello_world(){
    
	ERR_PRINT("Hello World!");
}