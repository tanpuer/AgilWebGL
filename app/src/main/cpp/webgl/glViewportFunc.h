//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLVIEWPORTFUNC_H
#define AGILV8_GLVIEWPORTFUNC_H

#include "v8.h"

auto glViewportFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 4) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glViewport Invalid argument"));
        return;
    }
    auto x = args[0].As<v8::Number>()->Int32Value();
    auto y = args[1].As<v8::Number>()->Int32Value();
    auto width = args[2].As<v8::Number>()->Int32Value();
    auto height = args[3].As<v8::Number>()->Int32Value();
    glViewport(x, y, width, height);
};

#endif //AGILV8_GLVIEWPORTFUNC_H
