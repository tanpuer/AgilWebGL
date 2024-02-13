#pragma once

#include "v8.h"
#include "AgilV8App.h"

auto registerSurfaceCreate = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    v8::HandleScope handleScope(isolate);
    if (args.Length() != 1 || !args[0]->IsFunction()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "invalid argument"));
        return;
    }
    auto data = v8::Local<v8::External>::Cast(args.Data());
    auto app = static_cast<AgilV8App *>(data->Value());
    assert(app);
    auto function = v8::Local<v8::Function>::Cast(args[0]);
    app->createCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
            isolate, function);
};

auto registerSurfaceDestroy = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    v8::HandleScope handleScope(isolate);
    if (args.Length() != 1 || !args[0]->IsFunction()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "invalid argument"));
        return;
    }
    auto data = v8::Local<v8::External>::Cast(args.Data());
    auto app = static_cast<AgilV8App *>(data->Value());
    assert(app);
    auto function = v8::Local<v8::Function>::Cast(args[0]);
    app->destroyCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
            isolate, function);
};
