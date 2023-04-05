#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <thread>


class Worker{
private:
    lua_State *Lua;
    bool ready;
    int result;
    std::thread *thread_handler;
public:
    Worker(lua_State *Lua):
        Lua(Lua)
    {
        luaL_openlibs(Lua);
    }

    ~Worker() {
        lua_close(Lua);
        delete thread_handler;
    }
public:
    void start() {
        thread_handler = new std::thread(
            [&Lua = this->Lua, &ready = this->ready, &result = this->result]() -> int {
                std::cout << "[wORKER Thread function] Start" << std::endl;
                std::cout << "[wORKER Thread function] Call" << std::endl;
                lua_call(Lua, 1, 1);
                std::cout << "[wORKER Thread function] Get result" << std::endl;
                result = lua_tointeger(Lua, 1);
                std::cout << "[wORKER Thread function] Pop result" << std::endl;
                lua_pop(Lua, 1);
                std::cout << "[wORKER Thread function] ready = true" << std::endl;
                ready = true;
                std::cout << "[wORKER Thread function] Return" << std::endl;
                return 0; 
            }
        );
    }

    bool get_ready() {
        return ready;
    }

    int get_result() {
        std::cout << "[LIB get_result function]" << std::endl;
        thread_handler->join();
        return result;
    }
};
