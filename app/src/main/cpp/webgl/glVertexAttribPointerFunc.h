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
    auto position = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto size = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    auto type = v8::Local<v8::Number>::Cast(args[2])->Int32Value();
    auto normalized = v8::Local<v8::Number>::Cast(args[3])->BooleanValue();
    auto stride = v8::Local<v8::Number>::Cast(args[4])->Int32Value();
    auto offset = v8::Local<v8::Number>::Cast(args[5])->Int32Value();
    glVertexAttribPointer(position, size, type, normalized, stride * sizeof(GLfloat), nullptr);
};

#endif //AGILV8_GLVERTEXATTRIBPOINTERFUNC_H
