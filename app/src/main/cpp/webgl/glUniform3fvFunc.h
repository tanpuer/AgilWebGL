//
// Created by banma-3412 on 2023/12/7.
//

#ifndef AGILV8_GLUNIFORM3FVFUNC_H
#define AGILV8_GLUNIFORM3FVFUNC_H

#include "v8.h"

auto glUniform3fvFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 3) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glUniform2fv Invalid argument"));
        return;
    }
    auto location = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    auto count = v8::Local<v8::Number>::Cast(args[1])->Int32Value();
    auto float32Array = args[2].As<v8::Float32Array>();
    v8::Local<v8::ArrayBuffer> buffer = float32Array->Buffer();
    v8::ArrayBuffer::Contents contents = buffer->GetContents();
    void *data = contents.Data();
    size_t offset = float32Array->ByteOffset();
    float *floats = static_cast<float *>(data) + offset / sizeof(float);
    glUniform3fv(location, count, floats);
};

#endif //AGILV8_GLUNIFORM3FVFUNC_H
