//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLCREATEBUFFERFUNC_H
#define AGILV8_GLCREATEBUFFERFUNC_H

#include "v8.h"

auto glCreateBufferFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 0) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glCreateBuffer Invalid argument"));
        return;
    }
    GLuint buffers[1] = {};
    glGenBuffers(1, buffers);
    args.GetReturnValue().Set(buffers[0]);
};

#endif //AGILV8_GLCREATEBUFFERFUNC_H
