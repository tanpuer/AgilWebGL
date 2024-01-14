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
    auto target = args[0].As<v8::Number>()->Int32Value();
    auto pname = args[1].As<v8::Number>()->Int32Value();
    auto param = args[2].As<v8::Number>()->Int32Value();
    glTexParameteri(target, pname, param);
};

#endif //AGILV8_GLTEXPARAMETERIFUNC_H
