//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLCREATETEXTUREFUNC_H
#define AGILV8_GLCREATETEXTUREFUNC_H

#include "v8.h"

auto glCreateTextureFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 0) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glCreateTextureFunc Invalid argument"));
        return;
    }
    auto texture = createTexture(GL_TEXTURE_2D);
    args.GetReturnValue().Set(texture);
};

#endif //AGILV8_GLCREATETEXTUREFUNC_H
