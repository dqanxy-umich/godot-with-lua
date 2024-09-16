#include "lua_engine.h"


void LuaEngine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("hello_world"), &LuaEngine::hello_world);
	ClassDB::bind_method(D_METHOD("do_string", "code"), &LuaEngine::do_string);
	ClassDB::bind_method(D_METHOD("add_module_dir", "dir"), &LuaEngine::add_module_dir);
}

std::vector<String> module_dirs;

std::vector<String> split_string(const String& str, char delimiter) {
	std::vector<String> tokens;
	std::string token;
	std::istringstream tokenStream(str.utf8().get_data());
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(String(token.c_str()));
	}
	return tokens;
}

//https://stackoverflow.com/questions/18965489/lua-require-but-files-are-only-in-memory you are a lifesaver

String LuaEngine::SearchScript(const char *name)
{
	std::vector<String> names = split_string(name, '.');

	for(int i = 0; i < module_dirs.size();i++){
		
		DirAccess *da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
		if(da->change_dir(module_dirs[i]) != OK){
			ERR_PRINT("Cannot change to directory: " + module_dirs[i]);
			continue;
		}

		for(int j = 0; j < names.size(); j++){
			if(j == names.size()-1){
				if(da->file_exists(names[j]+".lua")){
					WARN_PRINT("Found module: " + da->get_current_dir() + "/" + names[j]);
					String str = FileAccess::get_file_as_string(da->get_current_dir() + "/" + names[j] + ".lua") + "\n";
					return str;
				}
			}
			else{
				if(da->dir_exists(names[j])){
					da->change_dir(names[j]);
				}
				else{
					break;
				}
			}
		}
	}

	String directories = "";
	for(int i = 0; i < module_dirs.size();i++){
		directories += module_dirs[i] + "\n";
	}

	ERR_PRINT("Cannot find module: " + String(name) + " in module directories:\n" + directories);
	return "";
}

int LuaEngine::MyLoader(lua_State *L)
{
    const char *name = luaL_checkstring(L, 1);  // Module name

    String result = SearchScript(name); // Search your database.

    if(luaL_loadbuffer(L, result.ascii().get_data(), result.length(), name))
    {
		auto error = lua_tostring(L, lua_gettop(L));
       	ERR_PRINT(error);
        lua_pop(L, 1);
    }
    return 1;
}

void LuaEngine::SetLoader(lua_State* L)
{
    lua_register(L, "my_loader", LuaEngine::MyLoader);

    std::string str;
    str += "table.insert(package.searchers, 2, my_loader) \n";

    luaL_dostring(L, str.c_str());
}

LuaEngine::LuaEngine(){
	state = luaL_newstate();
	luaL_openlibs(state);
	//C Modules
    SetLoader(state);

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

void LuaEngine::_add_module_dir(String dir){
	module_dirs.push_back(dir);;
}

void LuaEngine::add_module_dir(String dir){
	LuaEngine::_add_module_dir(dir);;
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