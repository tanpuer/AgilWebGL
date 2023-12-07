//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLCREATESHADERFUNC_H
#define AGILV8_GLCREATESHADERFUNC_H

#include "v8.h"

auto glCreateShaderFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1 || !args[0]->IsNumber()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glCreateShader Invalid argument"));
        return;
    }
    auto type = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto result = glCreateShader(type);
    args.GetReturnValue().Set(result);
};

#endif //AGILV8_GLCREATESHADERFUNC_H
