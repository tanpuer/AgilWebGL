//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLGETATTRIBLOCATIONFUNC_H
#define AGILV8_GLGETATTRIBLOCATIONFUNC_H

#include "v8.h"
#include "gl_utils.h"

auto glGetAttribLocationFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glGetAttribLocation Invalid argument"));
        return;
    }
    auto program = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    v8::String::Utf8Value value(isolate, args[1]);
    auto nameStr = std::string(*value, value.length());
    const char *name = nameStr.c_str();
    auto position = glGetAttribLocation(program, name);
    name = nullptr;
    args.GetReturnValue().Set(position);
    if (position < 0) {
        checkGLError("glGetAttribLocation");
    }
};

#endif //AGILV8_GLGETATTRIBLOCATIONFUNC_H
