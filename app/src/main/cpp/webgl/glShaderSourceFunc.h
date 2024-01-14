//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLSHADERSOURCEFUNC_H
#define AGILV8_GLSHADERSOURCEFUNC_H

#include "v8.h"

auto glShaderSourceFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glShaderSource Invalid argument"));
        return;
    }
    auto shader = args[0].As<v8::Number>()->Int32Value();
    v8::String::Utf8Value value(isolate, args[1]);
    auto shaderStr = std::string(*value, value.length());
    const char *str = shaderStr.c_str();
    glShaderSource(shader, 1, &str, nullptr);
    str = nullptr;
};

#endif //AGILV8_GLSHADERSOURCEFUNC_H
