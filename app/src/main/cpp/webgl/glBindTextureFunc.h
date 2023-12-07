//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLBINDTEXTUREFUNC_H
#define AGILV8_GLBINDTEXTUREFUNC_H

#include "v8.h"

auto glBindTextureFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glBindTexture Invalid argument"));
        return;
    }
    auto target = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto texture = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    glBindTexture(target, texture);
};

#endif //AGILV8_GLBINDTEXTUREFUNC_H
