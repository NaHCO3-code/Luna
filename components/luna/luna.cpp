#include <stdio.h>
#include "luna.h"
#include "lunalib.h"
#include "esp_log.h"

static const char* TAG = "Luna";


esp_err_t luna::test(){
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  lua_register(L, "gpio_init", lunalib::gpio::init);
  lua_register(L, "gpio_set_level", lunalib::gpio::set_level);
  lua_register(L, "sleep", lunalib::sleep);

  if(luaL_dofile(L, "/spiffs/index.lua") != LUA_OK){
    ESP_LOGE(TAG, "Error loading file: %s", lua_tostring(L, -1));
  }

  lua_close(L);
  return ESP_OK;
}