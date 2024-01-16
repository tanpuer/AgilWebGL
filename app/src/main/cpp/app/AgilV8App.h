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

    static AssetManager *globalAssetManager;
    static long globalTime;

    int FRAME_INDEX = 0;
    std::map<int, v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>> frameCallbackMap;

private:

    std::shared_ptr<AssetManager> mAssetManager;
    std::unique_ptr<AgilV8Runtime> mV8Runtime;

    void injectBrowserAPI();

    void injectAgil();

    v8::Local<v8::Object> injectWebGL();

    std::unique_ptr<EGLCore> mEGLCore;
    int mWidth = 0, mHeight = 0;

    v8::Global<v8::Object> Agil;

};


#endif //AGILV8_AGILV8APP_H
