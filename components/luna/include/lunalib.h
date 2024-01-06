#pragma once
#include "lua.hpp"

namespace lunalib{
  int sleep(lua_State* L);
  namespace gpio{
    int init(lua_State* L);
    int set_level(lua_State* L);
  }
}