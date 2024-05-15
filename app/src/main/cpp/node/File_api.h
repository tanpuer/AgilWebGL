#pragma once

#include "v8.h"
#include "assert.h"
#include "File.h"

auto fileConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.IsConstructCall()) {
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto app = static_cast<AgilV8App *>(data->Value());
        assert(app);
        assert(args.Length() == 1 && args[0]->IsString());
        v8::String::Utf8Value str(isolate, args[0]);
        std::string path(*str);
        File *obj = new File(path, app);
        // 封装 C++ 对象
        v8::Local<v8::Object> self = args.Holder();
        self->SetInternalField(0, v8::External::New(isolate, obj));
    } else {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "File() must be called with new"));
    }
};

auto fileExit = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    assert(args.Length() == 0);
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(
            self->GetInternalField(0));
    void *ptr = wrap->Value();
    args.GetReturnValue().Set(static_cast<File *>(ptr)->exist());
};

auto fileRead = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    assert(args.Length() == 0);
    auto isolate = args.GetIsolate();
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(
            self->GetInternalField(0));
    void *ptr = wrap->Value();
    auto value = static_cast<File *>(ptr)->read();
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value.c_str()));
};
