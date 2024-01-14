//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLBUFFERDATAFUNC_H
#define AGILV8_GLBUFFERDATAFUNC_H

#include "v8.h"

auto glBufferDataFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 3 || !args[1]->IsFloat32Array()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glBufferData Invalid argument"));
        return;
    }
    auto target = args[0].As<v8::Number>()->Int32Value();;

    //todo 解析js传过来的floatArray
    auto float32Array = args[1].As<v8::Float32Array>();
    size_t length = float32Array->Length();
    v8::Local<v8::ArrayBuffer> buffer = float32Array->Buffer();
    v8::ArrayBuffer::Contents contents = buffer->GetContents();
    void *data = contents.Data();
    size_t offset = float32Array->ByteOffset();
    float *floats = static_cast<float *>(data) + offset / sizeof(float);

    auto usage = args[2].As<v8::Number>()->Int32Value();;
    glBufferData(target, sizeof(GLfloat) * length, floats, usage);
};

#endif //AGILV8_GLBUFFERDATAFUNC_H
