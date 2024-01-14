//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLVERTEXATTRIBPOINTERFUNC_H
#define AGILV8_GLVERTEXATTRIBPOINTERFUNC_H

#include "v8.h"

auto glVertexAttribPointerFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 6) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glVertexAttribPointer Invalid argument"));
        return;
    }
    auto position = args[0].As<v8::Number>()->Int32Value();
    auto size = args[1].As<v8::Number>()->Int32Value();
    auto type = args[2].As<v8::Number>()->Int32Value();
    auto normalized = args[3].As<v8::Number>()->Int32Value();
    auto stride = args[4].As<v8::Number>()->Int32Value();
    auto offset = args[5].As<v8::Number>()->Int32Value();
    glVertexAttribPointer(position, size, type, normalized, stride * sizeof(GLfloat), nullptr);
};

#endif //AGILV8_GLVERTEXATTRIBPOINTERFUNC_H
