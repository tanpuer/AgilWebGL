//
// Created by banma-3412 on 2023/12/7.
//

#ifndef AGILV8_GLUNIFORM1FFUNC_H
#define AGILV8_GLUNIFORM1FFUNC_H

#include "v8.h"

auto glUniform1fFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glUniform1i Invalid argument"));
        return;
    }
    auto location = args[0].As<v8::Number>()->Int32Value();
    auto value = args[1].As<v8::Number>()->Value();
    glUniform1f(location, value);
};

#endif //AGILV8_GLUNIFORM1FFUNC_H
