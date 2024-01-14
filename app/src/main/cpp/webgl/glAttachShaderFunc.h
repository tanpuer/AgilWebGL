//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLATTACHSHADERFUNC_H
#define AGILV8_GLATTACHSHADERFUNC_H

#include "v8.h"
#include "gl_utils.h"

auto glAttachShaderFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glAttachShader Invalid argument"));
        return;
    }
    auto program = args[0].As<v8::Number>()->Int32Value();;
    auto shader = args[1].As<v8::Number>()->Int32Value();;
    glAttachShader(program, shader);
    checkGLError("glAttachShader");
};

#endif //AGILV8_GLATTACHSHADERFUNC_H
