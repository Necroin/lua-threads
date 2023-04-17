#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <thread>


class Worker{
public:
    lua_State *Lua;
    bool ready;
    std::thread *thread_handler;
public:
    Worker(lua_State *Lua):
        Lua(Lua)
    {
        luaL_openlibs(Lua);
    }

    ~Worker() {
        delete thread_handler;
        lua_close(Lua);
    }
public:
    void start() {
        thread_handler = new std::thread(
            [&Lua = this->Lua, &ready = this->ready]() {
                std::cout << "[wORKER Thread function] Start" << std::endl;
                std::cout << "[wORKER Thread function] Lua top: " << lua_gettop(Lua) << std::endl;
                std::cout << "[wORKER Thread function] Call" << std::endl;
                lua_call(Lua, 1, 1);
                std::cout << "[wORKER Thread function] Lua top: " << lua_gettop(Lua) << std::endl;
                std::cout << "[wORKER Thread function] ready = true" << std::endl;
                ready = true;
                std::cout << "[wORKER Thread function] Return" << std::endl;
            }
        );
    }

    bool is_ready() {
        return ready;
    }

    void get_result(lua_State *other_Lua) {
        std::cout << "[LIB get_result function] Start" << std::endl;
        thread_handler->join();
        lua_xmove(this->Lua, other_Lua, 1);
        std::cout << "[LIB get_result function] Worker Lua top: " << lua_gettop(Lua) << std::endl;
    }
};
