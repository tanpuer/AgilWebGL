#pragma once

#include "v8.h"

auto glDeleteProgramFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1 || !args[0]->IsNumber()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glDeleteProgramFunc Invalid argument"));
        return;
    }
    auto program = args[0].As<v8::Number>()->Int32Value();
    glDeleteProgram(program);
};