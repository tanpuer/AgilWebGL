//
// Created by banma-3412 on 2023/12/2.
//
#define STB_IMAGE_IMPLEMENTATION

#include "AgilV8App.h"
#include "native_log.h"
#include "map"
#include "webgl/glClearColorFunc.h"
#include "webgl/glClearFunc.h"
#include "webgl/glCreateShaderFunc.h"
#include "webgl/glShaderSourceFunc.h"
#include "webgl/glCompileShaderFunc.h"
#include "webgl/glCreateProgramFunc.h"
#include "webgl/glAttachShaderFunc.h"
#include "webgl/glLinkProgramFunc.h"
#include "webgl/glGetUniformLocationFunc.h"
#include "webgl/glGetAttribLocationFunc.h"
#include "webgl/glEnableVertexAttribArrayFunc.h"
#include "webgl/glVertexAttribPointerFunc.h"
#include "webgl/glCreateBufferFunc.h"
#include "webgl/glBindBufferFunc.h"
#include "webgl/glBufferDataFunc.h"
#include "webgl/glViewportFunc.h"
#include "webgl/glUseProgramFunc.h"
#include "webgl/glGetShaderParameterFunc.h"
#include "webgl/glDeleteShaderFunc.h"
#include "webgl/glDrawArraysFunc.h"
#include "webgl/glCreateTextureFunc.h"
#include "webgl/glBindTextureFunc.h"
#include "webgl/glTexParameteriFunc.h"
#include "webgl/glTexImage2DFunc.h"
#include "webgl/glGenerateMipmapFunc.h"
#include "webgl/glUniform1iFunc.h"
#include "webgl/glActiveTextureFunc.h"
#include "webgl/glEnableFunc.h"
#include "webgl/glUniform2fvFunc.h"
#include "webgl/glUniform3fvFunc.h"
#include "webgl/glUniform1fFunc.h"
#include "browser/console.h"
#include "browser/timer.h"
#include "browser/animationFrame.h"

AssetManager *AgilV8App::globalAssetManager = nullptr;
long AgilV8App::globalTime = 0L;

AgilV8App::AgilV8App(JNIEnv *env, jobject javaAssetManager) {
    mAssetManager = std::make_shared<AssetManager>(env, javaAssetManager);
    globalAssetManager = mAssetManager.get();
}

AgilV8App::~AgilV8App() {
    globalAssetManager = nullptr;
}

void AgilV8App::create(ANativeWindow *window) {
    mV8Runtime = std::make_unique<AgilV8Runtime>(mAssetManager);
    injectBrowserAPI();
    injectWebGL();
    mEGLCore = std::make_unique<EGLCore>();
    mEGLCore->createGLEnv(nullptr, window, 0, 0, false);
    mEGLCore->makeCurrent();
}

void AgilV8App::change(int width, int height, long time) {
    globalTime = time;
    mWidth = width;
    mHeight = height;
    std::map<std::string, int> sizeMap = {
            {"drawingBufferWidth",  width},
            {"drawingBufferHeight", height},
    };
    mV8Runtime->injectNumberPropertiesToObject("gl", sizeMap);
    mEGLCore->swapBuffer();
}

void AgilV8App::doFrame(long time) {
    globalTime = time;
    if (mEGLCore == nullptr || FrameCallbackMap.empty()) {
        return;
    }
    for (auto callback: FrameCallbackMap) {
        const int argc = 0;
        v8::Local<v8::Value> argv[argc] = {};
        v8::Local<v8::Value> result;
        mV8Runtime->performFunction(callback.second, argc, argv, result);
    }
    mEGLCore->swapBuffer();
    checkGLError("doFrame");
}

void AgilV8App::destroy() {
    FrameCallbackMap.clear();
    mV8Runtime.reset(nullptr);
    mEGLCore.reset(nullptr);
}

bool AgilV8App::executeScript(const char *path, const char *moduleName) {
    std::string code = mAssetManager->readFile(path);
    auto result = mV8Runtime->evaluateJavaScript(code, moduleName);
    checkGLError("executeScript");
    return result;
}

void AgilV8App::injectBrowserAPI() {
    std::map<std::string, v8::FunctionCallback> consoleMap(
            {
                    {"log",   logCallback},
                    {"error", errorCallback},
                    {"info",  infoCallback},
                    {"warn",  warnCallback},
            }
    );
    mV8Runtime->injectObject("console", consoleMap, {});
    mV8Runtime->injectFunction("requestAnimationFrame", requestAnimationFrameCallback, nullptr);
    mV8Runtime->injectFunction("cancelAnimationFrame", cancelAnimationFrameCallback, nullptr);
}

void AgilV8App::injectWebGL() {
    std::map<std::string, v8::FunctionCallback> glMap(
            {
                    {"clearColor",              glClearColorFunc},
                    {"clear",                   glClearFunc},
                    {"createShader",            glCreateShaderFunc},
                    {"shaderSource",            glShaderSourceFunc},
                    {"compileShader",           glCompileShaderFunc},
                    {"createProgram",           glCreateProgramFunc},
                    {"attachShader",            glAttachShaderFunc},
                    {"linkProgram",             glLinkProgramFunc},
                    {"getAttribLocation",       glGetAttribLocationFunc},
                    {"enableVertexAttribArray", glEnableVertexAttribArrayFunc},
                    {"vertexAttribPointer",     glVertexAttribPointerFunc},
                    {"getUniformLocation",      glGetUniformLocationFunc},
                    {"createBuffer",            glCreateBufferFunc},
                    {"bindBuffer",              glBindBufferFunc},
                    {"bufferData",              glBufferDataFunc},
                    {"viewport",                glViewportFunc},
                    {"useProgram",              glUseProgramFunc},
                    {"getShaderParameter",      glGetShaderParameterFunc},
                    {"deleteShader",            glDeleteShaderFunc},
                    {"drawArrays",              glDrawArraysFunc},
                    {"createTexture",           glCreateTextureFunc},
                    {"bindTexture",             glBindTextureFunc},
                    {"activeTexture",           glActiveTextureFunc},
                    {"texParameteri",           glTexParameteriFunc},
                    {"texImage2D",              glTexImage2DFunc},
                    {"generateMipmap",          glGenerateMipmapFunc},
                    {"uniform1i",               glUniform1iFunc},
                    {"uniform1f",               glUniform1fFunc},
                    {"uniform2fv",              glUniform2fvFunc},
                    {"uniform3fv",              glUniform3fvFunc},
                    {"enable",                  glEnableFunc},
            }
    );
    std::map<std::string, int32_t> constMap(
            {
                    {"FLOAT",                GL_FLOAT},
                    {"COLOR_BUFFER_BIT",     GL_COLOR_BUFFER_BIT},
                    {"VERTEX_SHADER",        GL_VERTEX_SHADER},
                    {"FRAGMENT_SHADER",      GL_FRAGMENT_SHADER},
                    {"ARRAY_BUFFER",         GL_ARRAY_BUFFER},
                    {"ELEMENT_ARRAY_BUFFER", GL_ELEMENT_ARRAY_BUFFER},
                    {"STATIC_DRAW",          GL_STATIC_DRAW},
                    {"TRIANGLES",            GL_TRIANGLES},
                    {"TRIANGLE_STRIP",       GL_TRIANGLE_STRIP},
                    {"COMPILE_STATUS",       GL_COMPILE_STATUS},
                    {"TEXTURE_2D",           GL_TEXTURE_2D},
                    {"TEXTURE_WRAP_S",       GL_TEXTURE_WRAP_S},
                    {"TEXTURE_WRAP_T",       GL_TEXTURE_WRAP_T},
                    {"CLAMP_TO_EDGE",        GL_CLAMP_TO_EDGE},
                    {"TEXTURE_MIN_FILTER",   GL_TEXTURE_MIN_FILTER},
                    {"TEXTURE_MAG_FILTER",   GL_TEXTURE_MAG_FILTER},
                    {"LINEAR",               GL_LINEAR},
                    {"RGBA",                 GL_RGBA},
                    {"RGB",                  GL_RGB},
                    {"UNSIGNED_BYTE",        GL_UNSIGNED_BYTE},
                    {"TEXTURE0",             GL_TEXTURE0},
                    {"TEXTURE1",             GL_TEXTURE1},
                    {"TEXTURE2",             GL_TEXTURE2},
                    {"TEXTURE3",             GL_TEXTURE3},
                    {"NONE",                 GL_NONE},
                    {"BLEND",                GL_BLEND},
                    {"DEPTH_TEST",           GL_DEPTH_TEST},
                    {"STENCIL_TEST",         GL_STENCIL_TEST},
            }
    );
    mV8Runtime->injectObject("gl", glMap, constMap);
}
