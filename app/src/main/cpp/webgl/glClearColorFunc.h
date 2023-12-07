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
    auto r = static_cast<float >(v8::Local<v8::Number>::Cast(args[0])->Value());
    auto g = static_cast<float >(v8::Local<v8::Number>::Cast(args[1])->Value());
    auto b = static_cast<float >(v8::Local<v8::Number>::Cast(args[2])->Value());
    auto a = static_cast<float >(v8::Local<v8::Number>::Cast(args[3])->Value());
    glClearColor(r, g, b, a);
    checkGLError("glClearColor");
};

#endif //AGILV8_GLCLEARCOLORFUNC_H
