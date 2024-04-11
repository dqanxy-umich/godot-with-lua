#include "core/object.h"
#include "core/class_db.h"
extern "C"{
	#include <core/lua/lua.h>
	#include <core/lua/lauxlib.h>
	#include <core/lua/lualib.h>
}

class LuaEngine : public Object {
	GDCLASS(LuaEngine, Object);

private:
	lua_State *state;

protected:
	static void _bind_methods();

public:
	String do_string(String code);
    void hello_world();
	LuaEngine();
	~LuaEngine();
};