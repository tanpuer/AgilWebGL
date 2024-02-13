//
// Created by banma-3412 on 2023/12/2.
//

#ifndef AGILV8_AGILV8APP_H
#define AGILV8_AGILV8APP_H

#include "jni.h"
#include "AgilV8Runtime.h"
#include "AssetManager.h"
#include <android/looper.h>
#include <android/native_window_jni.h>
#include "map"
#include "EGLCore.h"

class AgilV8App {

public:

    AgilV8App(JNIEnv *env, jobject javaAssetManager);

    ~AgilV8App();

    bool executeScript(const char *path, const char *moduleName);

    void create(ANativeWindow *window);

    void change(int width, int height, long time);

    void destroy();

    void doFrame(long time);

public:
    
    long mGlobalTime = 0L;

    int FRAME_INDEX = 0;

    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> frameCallbackMap;

    std::shared_ptr<AssetManager> mAssetManager;

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> createCallback;

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> destroyCallback;

private:

    std::unique_ptr<AgilV8Runtime> mV8Runtime;

    void injectBrowserAPI();

    void injectAgil();

    v8::Local<v8::Object> injectWebGL();

    std::unique_ptr<EGLCore> mEGLCore;

    v8::Global<v8::Object> mAgil;

};


#endif //AGILV8_AGILV8APP_H
