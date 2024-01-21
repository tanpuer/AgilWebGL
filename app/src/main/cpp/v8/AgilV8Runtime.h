//
// Created by banma-3412 on 2023/12/2.
//

#ifndef AGILV8_AGILV8RUNTIME_H
#define AGILV8_AGILV8RUNTIME_H

#include "memory"
#include "v8.h"
#include "v8-platform.h"
#include "AssetManager.h"
#include "map"

class AgilV8Runtime {

public:

    AgilV8Runtime(std::shared_ptr<AssetManager> assetManager);

    ~AgilV8Runtime();

    bool evaluateJavaScript(const std::string &buffer, const std::string &sourceURL);

    v8::Isolate *getIsolate() const;

    v8::Local<v8::Object> injectObject(v8::Local<v8::Object> host, const char *name,
                                       std::map<std::string, v8::FunctionCallback> functionMap,
                                       std::map<std::string, int32_t> constMap,
                                       void *any = nullptr);

    v8::Local<v8::Object> injectObject(v8::Local<v8::Object> host, const char *name,
                                       std::map<std::string, v8::FunctionCallback> functionMap,
                                       std::map<std::string, std::string> constMap,
                                       void *any = nullptr);

    void injectFunction(const char *name, v8::FunctionCallback callback, void *any);

    v8::Local<v8::Value>
    performFunction(
            v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> function,
            int argc, v8::Local<v8::Value> argv[]);

    void injectNumberPropertiesToObject(v8::Local<v8::Object> host, const char *name, std::map<std::string, int> properties);

    void injectDate(std::function<void(v8::Isolate *)> registerObject);

    std::string utf8(const v8::Local<v8::Value> &value);

    v8::Local<v8::Object> global();

private:

    std::shared_ptr<AssetManager> mAssetManager;
    v8::Isolate *mIsolate = nullptr;
    std::unique_ptr<v8::ArrayBuffer::Allocator> arrayBufferAllocator_;
    v8::Global<v8::Context> mContext;
    std::unique_ptr<v8::Platform> mPlatform;

    v8::Local<v8::Context> CreateGlobalContext(v8::Isolate *isolate);

    bool executeScript(const v8::Local<v8::String> &script, const std::string &sourceURL);

    void ReportException(v8::TryCatch *tryCatch) const;

};


#endif //AGILV8_AGILV8RUNTIME_H
