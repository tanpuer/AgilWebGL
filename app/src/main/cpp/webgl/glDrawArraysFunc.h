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
    auto mode = args[0].As<v8::Number>()->Int32Value();
    auto first = args[1].As<v8::Number>()->Int32Value();
    auto size = args[2].As<v8::Number>()->Int32Value();
    glDrawArrays(mode, first, size);
};

#endif //AGILV8_GLDRAWARRAYSFUNC_H
