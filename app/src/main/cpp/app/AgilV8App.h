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

private:

    std::shared_ptr<AssetManager> mAssetManager;
    std::unique_ptr<AgilV8Runtime> mV8Runtime;

    void injectBrowserAPI();

    void injectWebGL();

    std::unique_ptr<EGLCore> mEGLCore;
    int mWidth = 0, mHeight = 0;
};


#endif //AGILV8_AGILV8APP_H
