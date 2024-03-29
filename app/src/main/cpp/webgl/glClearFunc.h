//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLCLEARFUNC_H
#define AGILV8_GLCLEARFUNC_H

#include "v8.h"

auto glClearFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glClear Invalid argument"));
        return;
    }
    auto mask = args[0].As<v8::Number>()->Int32Value();;
    glClear(mask);
};

#endif //AGILV8_GLCLEARFUNC_H
