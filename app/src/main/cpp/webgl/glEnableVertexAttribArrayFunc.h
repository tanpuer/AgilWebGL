//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLENABLEVERTEXATTRIBARRAYFUNC_H
#define AGILV8_GLENABLEVERTEXATTRIBARRAYFUNC_H

#include "v8.h"

auto glEnableVertexAttribArrayFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glEnableVertexAttribArray Invalid argument"));
        return;
    }
    auto position = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    glEnableVertexAttribArray(position);
};

#endif //AGILV8_GLENABLEVERTEXATTRIBARRAYFUNC_H
