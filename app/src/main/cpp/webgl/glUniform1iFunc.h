//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLUNIFORM1IFUNC_H
#define AGILV8_GLUNIFORM1IFUNC_H

#include "v8.h"

auto glUniform1iFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glUniform1i Invalid argument"));
        return;
    }
    auto location = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto value = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    glUniform1i(location, value);
};

#endif //AGILV8_GLUNIFORM1IFUNC_H
