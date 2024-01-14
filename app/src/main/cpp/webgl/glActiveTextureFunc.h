//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLACTIVETEXTUREFUNC_H
#define AGILV8_GLACTIVETEXTUREFUNC_H

#include "v8.h"

auto glActiveTextureFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glActiveTexture Invalid argument"));
        return;
    }
    auto texture = args[0].As<v8::Number>()->Int32Value();
    glActiveTexture(texture);
};

#endif //AGILV8_GLACTIVETEXTUREFUNC_H
