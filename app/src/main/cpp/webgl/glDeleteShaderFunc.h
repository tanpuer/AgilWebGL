//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLDELETESHADERFUNC_H
#define AGILV8_GLDELETESHADERFUNC_H

#include "v8.h"

auto glDeleteShaderFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1 || !args[0]->IsNumber()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glDeleteShaderFunc Invalid argument"));
        return;
    }
    auto shader = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    glDeleteShader(shader);
};

#endif //AGILV8_GLDELETESHADERFUNC_H
