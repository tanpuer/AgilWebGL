//
// Created by banma-3412 on 2023/12/2.
//

#include "AgilV8Runtime.h"
#include "native_log.h"
#include "libplatform/libplatform.h"

AgilV8Runtime::AgilV8Runtime(std::shared_ptr<AssetManager> assetManager) {
    mAssetManager = assetManager;
    v8::V8::SetFlagsFromString("--nolazy");
    v8::V8::Initialize();
    arrayBufferAllocator_.reset(
            v8::ArrayBuffer::Allocator::NewDefaultAllocator());
    mPlatform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializeICU();
    v8::V8::InitializePlatform(mPlatform.get());
    v8::Isolate::CreateParams createParams;
    createParams.array_buffer_allocator = arrayBufferAllocator_.get();
    mIsolate = v8::Isolate::New(createParams);
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    mContext.Reset(mIsolate, CreateGlobalContext(mIsolate));
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    ALOGD("AgilV8Runtime init success")
}

AgilV8Runtime::~AgilV8Runtime() {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    mContext.Reset();
    mIsolate->Dispose();
}

v8::Local<v8::Context> AgilV8Runtime::CreateGlobalContext(v8::Isolate *isolate) {
    v8::EscapableHandleScope scopedHandle(isolate);
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    return scopedHandle.Escape(v8::Context::New(isolate, nullptr, global));
}

bool AgilV8Runtime::evaluateJavaScript(const std::string &buffer, const std::string &sourceURL) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    v8::Local<v8::String> string;
    v8::String::NewFromUtf8(
            mIsolate,
            reinterpret_cast<const char *>(buffer.c_str()),
            v8::NewStringType::kNormal,
            buffer.size()).ToLocal(&string);
    return executeScript(string, sourceURL);
}

bool
AgilV8Runtime::executeScript(const v8::Local<v8::String> &script, const std::string &sourceURL) {
    v8::HandleScope scopedHandle(mIsolate);
    v8::TryCatch tryCatch(mIsolate);
    v8::MaybeLocal<v8::String> sourceURLValue = v8::String::NewFromUtf8(
            mIsolate,
            sourceURL.c_str(),
            v8::NewStringType::kNormal,
            static_cast<int>(sourceURL.length()));
    v8::ScriptOrigin origin(sourceURLValue.ToLocalChecked());
    v8::Local<v8::Context> context(mIsolate->GetCurrentContext());
    std::unique_ptr<v8::ScriptCompiler::Source> source =
            std::make_unique<v8::ScriptCompiler::Source>(script, origin);
    v8::Local<v8::Script> compiledScript;
    if (!v8::ScriptCompiler::Compile(
            context,
            source.release(),
            v8::ScriptCompiler::kNoCompileOptions)
            .ToLocal(&compiledScript)) {
        ReportException(&tryCatch);
        return false;
    }
    v8::Local<v8::Value> result;
    if (!compiledScript->Run(context).ToLocal(&result)) {
        ReportException(&tryCatch);
        return false;
    }
    ALOGD("AgilV8Runtime::executeScript success")
    return true;
}

void AgilV8Runtime::ReportException(v8::TryCatch *tryCatch)
const {
    v8::HandleScope scopedHandle(mIsolate);
    v8::Local<v8::Message> message = tryCatch->Message();
    v8::String::Utf8Value utf8(mIsolate, message->Get());
    auto error = std::string(*utf8, utf8.length());
    ALOGD("AgilV8Runtime::ReportException %s", error.c_str())
}

v8::Isolate *AgilV8Runtime::getIsolate() const {
    return mIsolate;
}

void AgilV8Runtime::injectObject(const char *name,
                                 std::map<std::string, v8::FunctionCallback> functionMap,
                                 std::map<std::string, int32_t> constMap) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    v8::Local<v8::Object> object = v8::Object::New(mIsolate);
    for (std::pair<std::string, v8::FunctionCallback> pair: functionMap) {
        auto function = v8::FunctionTemplate::New(mIsolate, pair.second)->GetFunction();
        auto result = object->Set(v8::String::NewFromUtf8(mIsolate, pair.first.c_str()), function);
        ALOGD("%s set %s result: %d", name, pair.first.c_str(), result)
    }
    for (std::pair<std::string, int32_t> pair: constMap) {
        object->Set(v8::String::NewFromUtf8(mIsolate, pair.first.c_str()),
                    v8::Number::New(mIsolate, pair.second));
    }
    auto global = mContext.Get(mIsolate)->Global();
    auto result = global->Set(v8::String::NewFromUtf8(mIsolate, name), object);
    ALOGD("global set object: %s result: %d", name, result)
}

void AgilV8Runtime::injectFunction(const char *name, v8::FunctionCallback callback, void *any) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    v8::Local<v8::External> external_context_data =
            v8::External::New(mIsolate, any);
    v8::Local<v8::FunctionTemplate> funcTemplate = v8::FunctionTemplate::New(mIsolate, callback,
                                                                             external_context_data);
    v8::Local<v8::Function> function = funcTemplate->GetFunction();
    v8::Local<v8::String> funcName = v8::String::NewFromUtf8(mIsolate, name);
    auto global = mContext.Get(mIsolate)->Global();
    auto result = global->Set(funcName, function);
    ALOGD("global set function: %s result: %d", name, result)
}

v8::Local<v8::Value>
AgilV8Runtime::performFunction(
        v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> function, int argc,
        v8::Local<v8::Value> argv[], v8::Local<v8::Value> result) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));

    v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(mIsolate, function);
    v8::TryCatch try_catch(mIsolate);
    callback->Call(mIsolate->GetCurrentContext(), mIsolate->GetCurrentContext()->Global(), 0,
                   nullptr);
    if (try_catch.HasCaught()) {
        v8::String::Utf8Value exception(mIsolate, try_catch.Exception());
        auto info = std::string(*exception, exception.length());
        ALOGD("performFunction error %s", info.c_str())
    }
    return result;
}

void AgilV8Runtime::injectNumberPropertiesToObject(const char *name,
                                                   std::map<std::string, int> properties) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));

    v8::Local<v8::Value> injectedObj;
    auto objName = v8::String::NewFromUtf8(mIsolate, name);
    auto global = mContext.Get(mIsolate)->Global();
    if (!global->Has(objName)) {
        ALOGE("v8 does not inject object:%s", name)
        return;
    }
    auto obj = global->Get(objName).As<v8::Object>();
    for (auto pair: properties) {
        obj->Set(v8::String::NewFromUtf8(mIsolate, pair.first.c_str()),
                 v8::Number::New(mIsolate, pair.second));
    }
}

void AgilV8Runtime::injectDate(std::function<void(v8::Isolate*)> registerObject) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    registerObject(mIsolate);
}
