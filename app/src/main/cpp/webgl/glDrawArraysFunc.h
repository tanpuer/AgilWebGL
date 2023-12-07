//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLDRAWARRAYSFUNC_H
#define AGILV8_GLDRAWARRAYSFUNC_H

#include "v8.h"

auto glDrawArraysFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 3) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glDrawArraysFunc Invalid argument"));
        return;
    }
    auto mode = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto first = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    auto size = v8::Local<v8::Number>::Cast(args[2])->Int32Value();
    glDrawArrays(mode, first, size);
};

#endif //AGILV8_GLDRAWARRAYSFUNC_H
