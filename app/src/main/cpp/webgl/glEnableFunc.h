//
// Created by banma-3412 on 2023/12/7.
//

#ifndef AGILV8_GLENABLEFUNC_H
#define AGILV8_GLENABLEFUNC_H

#include "v8.h"

auto glEnableFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glEnable Invalid argument"));
        return;
    }
    auto cap = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    glEnable(cap);
};

#endif //AGILV8_GLENABLEFUNC_H
