#include "glsl_validate.hpp"
#include <napi.h>

Napi::Object init(Napi::Env env, Napi::Object exports) {
    if (node_glslang::init()) {
        exports.Set(Napi::String::New(env, "compiles"), Napi::Function::New(env, node_glslang::compiles));
    }
    return exports;
}

NODE_API_MODULE(module, init)