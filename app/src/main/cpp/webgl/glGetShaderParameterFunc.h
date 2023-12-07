//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLGETSHADERPARAMETERFUNC_H
#define AGILV8_GLGETSHADERPARAMETERFUNC_H

#include "v8.h"

auto glGetShaderParameterFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glGetShaderParameterFunc Invalid argument"));
        return;
    }
    GLint compiled;
    auto shader = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto pName = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    glGetShaderiv(shader, pName, &compiled);
    args.GetReturnValue().Set(compiled);
};

#endif //AGILV8_GLGETSHADERPARAMETERFUNC_H
