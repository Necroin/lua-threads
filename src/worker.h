#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <thread>

#include "debug.h"

class Worker
{
public:
    lua_State *Lua;
    bool ready;
    std::thread *thread_handler;

public:
    Worker(lua_State *Lua) : Lua(Lua)
    {
        luaL_openlibs(Lua);
    }

    ~Worker()
    {
        delete thread_handler;
        lua_close(Lua);
    }

public:
    void start()
    {
        thread_handler = new std::thread(
            [&Lua = this->Lua, &ready = this->ready]()
            {
#if DEBUG
                std::cout << "[Threads C++ LIB] [DEBUG] [wORKER Thread function] Start" << std::endl;
                std::cout << "[Threads C++ LIB] [DEBUG] [wORKER Thread function] Lua top: " << lua_gettop(Lua) << std::endl;
                std::cout << "[Threads C++ LIB] [DEBUG] [wORKER Thread function] Call" << std::endl;
#endif
                lua_call(Lua, 1, 1);
#if DEBUG
                std::cout << "[Threads C++ LIB] [DEBUG] [wORKER Thread function] Lua top: " << lua_gettop(Lua) << std::endl;
                std::cout << "[Threads C++ LIB] [DEBUG] [wORKER Thread function] ready = true" << std::endl;
#endif

                ready = true;
#if DEBUG
                std::cout << "[Threads C++ LIB] [DEBUG] [wORKER Thread function] Return" << std::endl;
#endif
            });
    }

    bool is_ready()
    {
        return ready;
    }

    void get_result(lua_State *other_Lua)
    {
#if DEBUG
        std::cout << "[Threads C++ LIB] [DEBUG] [LIB get_result function] Start" << std::endl;
#endif

        thread_handler->join();
        lua_xmove(this->Lua, other_Lua, 1);
#if DEBUG
        std::cout << "[Threads C++ LIB] [DEBUG] [LIB get_result function] Worker Lua top: " << lua_gettop(Lua) << std::endl;
#endif
    }
};
