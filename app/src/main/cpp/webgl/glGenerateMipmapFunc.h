//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLGENERATEMIPMAPFUNC_H
#define AGILV8_GLGENERATEMIPMAPFUNC_H

#include "v8.h"

auto glGenerateMipmapFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glGenerateMipmap Invalid argument"));
        return;
    }
    auto target = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    glGenerateMipmap(target);
};

#endif //AGILV8_GLGENERATEMIPMAPFUNC_H
