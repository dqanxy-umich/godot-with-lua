#include "core/object.h"
#include "core/class_db.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include <vector>
#include <sstream>

extern "C"{
	#include <core/lua/lua.h>
	#include <core/lua/lauxlib.h>
	#include <core/lua/lualib.h>
	#include <core/lua/protobuf/pb.h>
}

class LuaEngine : public Object {
	GDCLASS(LuaEngine, Object);

private:
	lua_State *state;
	static void LuaEngine::SetLoader(lua_State* L);
	static int LuaEngine::MyLoader(lua_State *L);
	static String LuaEngine::SearchScript(const char *name);

protected:
	static void _bind_methods();

public:
	String do_string(String code);
    void hello_world();
	static void _add_module_dir(String dir);
	void add_module_dir(String dir);
	LuaEngine();
	~LuaEngine();
};