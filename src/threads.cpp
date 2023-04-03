#include <thread>
#include <iostream>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

extern "C" int LIB_check_result(lua_State *Lua)
{
    void *ready_pointer = lua_touserdata(Lua, 1);
    bool* casted_ready_pointer = (bool*)ready_pointer;
    lua_pushboolean(Lua, *casted_ready_pointer);
    return 1;
}

extern "C" int LIB_get_result(lua_State *Lua)
{
    void *thread_pointer = lua_touserdata(Lua, 1);
    std::thread *casted_thread_pointer = (std::thread *)thread_pointer;
    casted_thread_pointer->join();
    //delete casted_thread_pointer;
    // lua_pushboolean(Lua, 1);
    return 1;
}

extern "C" int LIB_async(lua_State *Lua)
{
    std::cout << "LIB_async" << std::endl;
    lua_newtable(Lua);
    lua_pushcfunction(Lua, &LIB_get_result);
    lua_setfield(Lua, -2, "get");
    lua_pushcfunction(Lua, &LIB_check_result);
    lua_setfield(Lua, -2, "check");
    bool* ready = new bool(false);
    lua_pushlightuserdata(Lua, ready);
    lua_setfield(Lua, -2, "ready_pointer");
    std::thread *thread_handler = new std::thread(
        [&Lua = Lua, &ready]() -> int {
            std::cout << "[LIB Thread function]" << std::endl;
            lua_call(Lua, 1, 1);
            std::cout << "[LIB Thread function] ready = true" << std::endl;
            *ready = true;
            std::cout << "[LIB Thread function] retrun" << std::endl;
            return 1; 
        }
    );
    lua_pushlightuserdata(Lua, (void *)thread_handler);
    lua_setfield(Lua, -2, "thread_pointer");

    return 1;
}

void init(lua_State *Lua)
{
    lua_register(Lua, "LIB_async", LIB_async);
}

extern "C" int luaopen_libthreads(lua_State *Lua)
{
    lua_newtable(Lua);
    lua_pushcfunction(Lua, &LIB_async);
    lua_setfield(Lua, -2, "LIB_async");

    return 1;
}