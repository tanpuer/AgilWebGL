//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLTEXPARAMETERIFUNC_H
#define AGILV8_GLTEXPARAMETERIFUNC_H

#include "v8.h"

auto glTexParameteriFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 3) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glTexParameteri Invalid argument"));
        return;
    }
    auto target = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto pname = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    auto param = v8::Local<v8::Number>::Cast(args[2])->Int32Value();
    glTexParameteri(target, pname, param);
};

#endif //AGILV8_GLTEXPARAMETERIFUNC_H
