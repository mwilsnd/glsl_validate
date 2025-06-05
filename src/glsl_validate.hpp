#pragma once
#include <napi.h>

namespace node_glslang {

bool init();
Napi::Value compiles(Napi::CallbackInfo const& info);

}