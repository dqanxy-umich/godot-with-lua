#include "core/object.h"
#include "core/class_db.h"

class LuaEngine : public Object {
	GDCLASS(LuaEngine, Object);

protected:
	static void _bind_methods();

public:
    void hello_world();
};