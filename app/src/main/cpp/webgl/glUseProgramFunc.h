//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLUSEPROGRAMFUNC_H
#define AGILV8_GLUSEPROGRAMFUNC_H

#include "v8.h"

auto glUseProgramFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glUseProgram Invalid argument"));
        return;
    }
    auto program = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    glUseProgram(program);
};

#endif //AGILV8_GLUSEPROGRAMFUNC_H
