#include <thread>
#include <iostream>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "worker.h"

#define DEBUG 1
#define READY_DEBUG 0

extern "C" int check_result(lua_State *Lua)
{
    decltype(auto) worker = (Worker*)lua_touserdata(Lua, 1);
    if (worker == NULL) {
        throw std::runtime_error("ready_pointer is NULL");
    }
    lua_pushboolean(Lua, worker->get_ready());
    return 1;
}

extern "C" int get_result(lua_State *Lua)
{
    decltype(auto) worker = (Worker*)lua_touserdata(Lua, 1);
    if (worker == NULL) {
        throw std::runtime_error("ready_pointer is NULL");
    }
    lua_pushinteger(Lua, worker->get_result());
    delete worker;
    return 1;
}

extern "C" int LIB_async(lua_State *Lua)
{
    std::cout << "[LIB_async function] Start" << std::endl;
    lua_State* newLua = luaL_newstate();
    lua_xmove(Lua, newLua, 2);
    decltype(auto) worker = new Worker(newLua);

    lua_newtable(Lua);
    lua_pushcfunction(Lua, &get_result);
    lua_setfield(Lua, -2, "get");
    lua_pushcfunction(Lua, &check_result);
    lua_setfield(Lua, -2, "check");
    lua_pushlightuserdata(Lua, worker);
    lua_setfield(Lua, -2, "worker");

    worker->start();
    std::cout << "[LIB_async function] End" << std::endl;
    return 1;
}

extern "C" int luaopen_libthreads(lua_State *Lua)
{
    lua_newtable(Lua);
    lua_pushcfunction(Lua, &LIB_async);
    lua_setfield(Lua, -2, "LIB_async");

    return 1;
}