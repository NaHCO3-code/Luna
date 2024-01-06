#include "lunalib.h"
extern "C"{
  #include "driver/gpio.h"
  #include "esp_log.h"
  #include <sys/unistd.h>
}

static const char *TAG = "lunalib";

int64_t get_int(lua_State *L, int i, int64_t min, int64_t max){
  if(!lua_isinteger(L, i)){
    luaL_error(L, "Expected integer");
    ESP_LOGE(TAG, "Expected integer");
    return 0;
  }
  int64_t num = lua_tointeger(L, i);
  if(num > max || num < min){
    luaL_error(L, "Expected integer between %lld and %lld", min, max);
    return 0;
  }
  return num;
}

bool get_bool(lua_State* L, int i){
  if(!lua_isboolean(L, i)){
    luaL_error(L, "Expected boolean");
    ESP_LOGE(TAG, "Expected boolean");
    return false;
  }
  return lua_toboolean(L, i)==0?false:true;
}

int lunalib::sleep(lua_State* L){
  auto ms = get_int(L, 1, 0, 9223372036854775807);
  usleep(ms*1000);
  return 0;
}

int lunalib::gpio::init(lua_State* L){
  auto gpio_num = get_int(L, 1, 0, 40);
  bool mode = get_bool(L, 2);
  bool pullup = get_bool(L, 3);
  bool pulldown = get_bool(L, 4);

  gpio_config_t conf = {
    .pin_bit_mask = (1ULL<<gpio_num),
    .mode = mode ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT,
    .pull_up_en = pullup ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
    .pull_down_en = pulldown ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };

  esp_err_t ret = gpio_config(&conf);
  if(ret!= ESP_OK){
    luaL_error(L, "gpio_config failed");
  }
 
  return 0;
}

int lunalib::gpio::set_level(lua_State* L){
  auto gpio_num = (gpio_num_t)get_int(L, 1, 0, 40);
  bool level = get_bool(L, 2);
  esp_err_t ret = gpio_set_level(gpio_num, level);
  if(ret != ESP_OK){
    luaL_error(L, "gpio_set_level failed");
  }
  return 0;
}