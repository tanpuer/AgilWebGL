//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLBINDBUFFERFUNC_H
#define AGILV8_GLBINDBUFFERFUNC_H

#include "v8.h"

auto glBindBufferFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glBindBuffer Invalid argument"));
        return;
    }
    auto target = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto buffer = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    glBindBuffer(target, buffer);
};

#endif //AGILV8_GLBINDBUFFERFUNC_H
