//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLCREATEPROGRAMFUNC_H
#define AGILV8_GLCREATEPROGRAMFUNC_H

#include "v8.h"

auto glCreateProgramFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 0) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glCreateProgram Invalid argument"));
        return;
    }
    auto program = glCreateProgram();
    args.GetReturnValue().Set(program);
};

#endif //AGILV8_GLCREATEPROGRAMFUNC_H
