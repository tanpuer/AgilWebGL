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
    auto x = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto y = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    auto width = v8::Local<v8::Number>::Cast(args[2])->Int32Value();
    auto height = v8::Local<v8::Number>::Cast(args[3])->Int32Value();
    glViewport(x, y, width, height);
};

#endif //AGILV8_GLVIEWPORTFUNC_H
