#include <thread>
#include <iostream>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "worker.h"

#include "debug.h"

extern "C" int finish(lua_State *Lua)
{
#if DEBUG
    std::cout << "[Threads C++ LIB] [DEBUG] called finish function" << std::endl;
#endif
    decltype(auto) worker = (Worker *)lua_touserdata(Lua, 1);
#if DEBUG
    std::cout << "[Threads C++ LIB] [DEBUG] worker address: " << worker << std::endl;
#endif
    if (worker == NULL)
    {
        throw std::runtime_error("[Threads C++ LIB] [Error] worker is NULL");
    }
    delete worker;
    lua_pushboolean(Lua, true);
    return 1;
}

extern "C" int check_result(lua_State *Lua)
{
#if DEBUG_CHECK
    std::cout << "[Threads C++ LIB] [DEBUG] called check_result function" << std::endl;
#endif
    decltype(auto) worker = (Worker *)lua_touserdata(Lua, 1);
#if DEBUG_CHECK
    std::cout << "[Threads C++ LIB] [DEBUG] worker address: " << worker << std::endl;
#endif
    if (worker == NULL)
    {
        throw std::runtime_error("[Threads C++ LIB] [Error] worker is NULL");
    }
#if DEBUG_CHECK
    std::cout << "[Threads C++ LIB] [DEBUG] worker is_ready: " << worker->is_ready() << std::endl;
#endif
    lua_pushboolean(Lua, worker->is_ready());
    return 1;
}

extern "C" int get_result(lua_State *Lua)
{
#if DEBUG
    std::cout << "[Threads C++ LIB] [DEBUG] called get_result function" << std::endl;
#endif
    decltype(auto) worker = (Worker *)lua_touserdata(Lua, 1);
#if DEBUG
    std::cout << "[Threads C++ LIB] [DEBUG] worker address: " << worker << std::endl;
#endif
    if (worker == NULL)
    {
        throw std::runtime_error("[Threads C++ LIB] [Error] worker is NULL");
    }
    if (worker->is_ready() == false)
    {
        lua_pushnil(Lua);
        return 1;
    }
    worker->get_result(Lua);
    return 1;
}

extern "C" int LIB_async(lua_State *Lua)
{
#if DEBUG
    std::cout << "[Threads C++ LIB] called LIB_async function" << std::endl;
#endif
    lua_State *newLua = luaL_newstate();
    lua_xmove(Lua, newLua, 2);
    decltype(auto) worker = new Worker(newLua);

    lua_newtable(Lua);
    lua_pushcfunction(Lua, &get_result);
    lua_setfield(Lua, -2, "get");
    lua_pushcfunction(Lua, &check_result);
    lua_setfield(Lua, -2, "check");
    lua_pushcfunction(Lua, &finish);
    lua_setfield(Lua, -2, "finish");
    lua_pushlightuserdata(Lua, worker);
    lua_setfield(Lua, -2, "worker");

    worker->start();
#if DEBUG
    std::cout << "[Threads C++ LIB] LIB_async function finished" << std::endl;
#endif
    return 1;
}

extern "C" int luaopen_libthreads(lua_State *Lua)
{
    lua_newtable(Lua);
    lua_pushcfunction(Lua, &LIB_async);
    lua_setfield(Lua, -2, "LIB_async");
    return 1;
}