//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLCLEARCOLORFUNC_H
#define AGILV8_GLCLEARCOLORFUNC_H

#include "v8.h"
#include "base/gl_utils.h"

auto glClearColorFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 4) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glClearColor Invalid argument"));
        return;
    }
    auto r = static_cast<float >(args[0].As<v8::Number>()->Value());
    auto g = static_cast<float >(args[1].As<v8::Number>()->Value());
    auto b = static_cast<float >(args[2].As<v8::Number>()->Value());
    auto a = static_cast<float >(args[3].As<v8::Number>()->Value());
    glClearColor(r, g, b, a);
    checkGLError("glClearColor");
};

#endif //AGILV8_GLCLEARCOLORFUNC_H
