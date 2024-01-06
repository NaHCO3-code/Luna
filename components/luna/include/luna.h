#pragma once
#include "lua.hpp"
extern "C" {
  #include "esp_err.h"
}

namespace luna{
  esp_err_t init();
  esp_err_t test();
}