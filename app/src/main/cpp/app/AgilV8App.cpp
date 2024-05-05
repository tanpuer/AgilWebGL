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
#include "webgl/glDeleteProgram.h"
#include "browser/console.h"
#include "browser/timer.h"
#include "browser/animationFrame.h"
#include "browser/surface.h"
#include "File.h"

AgilV8App::AgilV8App(JNIEnv *env, jobject javaAssetManager) {
    mAssetManager = std::make_shared<AssetManager>(env, javaAssetManager);
}

AgilV8App::~AgilV8App() {
    if (mEGLCore != nullptr) {
        mEGLCore.reset(nullptr);
    }
    frameCallbackMap.clear();
    mV8Runtime.reset(nullptr);
}

bool AgilV8App::executeScript(const char *path, const char *moduleName) {
    mV8Runtime = std::make_unique<AgilV8Runtime>(mAssetManager);
    injectBrowserAPI();
    injectAgil();
    injectNodeApi();
    std::string code = mAssetManager->readFile(path);
    auto result = mV8Runtime->evaluateJavaScript(code, moduleName);
    checkGLError("executeScript");
    return result;
}

void AgilV8App::create(ANativeWindow *window) {
    mEGLCore = std::make_unique<EGLCore>();
    mEGLCore->createGLEnv(nullptr, window, 0, 0, false);
    mEGLCore->makeCurrent();
    if (!createCallback.IsEmpty()) {
        mV8Runtime->performFunction(createCallback, 0, nullptr);
    }
}

void AgilV8App::change(int width, int height, long time) {
    mGlobalTime = time;
    std::map<std::string, int> sizeMap = {
            {"drawingBufferWidth",  width},
            {"drawingBufferHeight", height},
    };
    mV8Runtime->injectNumberPropertiesToObject(mV8Runtime->global(), "gl", sizeMap);
    mEGLCore->swapBuffer();
}

void AgilV8App::doFrame(long time) {
    if (mEGLCore == nullptr) {
        return;
    }
    mGlobalTime = time;
    if (mEGLCore == nullptr || frameCallbackMap.empty()) {
        return;
    }
    for (auto callback: frameCallbackMap) {
        const int argc = 0;
        v8::Local<v8::Value> argv[argc] = {};
        mV8Runtime->performFunction(callback.second, argc, argv);
    }
    mEGLCore->swapBuffer();
    checkGLError("doFrame");
}

void AgilV8App::destroy() {
    if (!destroyCallback.IsEmpty()) {
        mV8Runtime->performFunction(destroyCallback, 0, nullptr);
    }
    mEGLCore.reset(nullptr);
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
    mV8Runtime->injectObject(mV8Runtime->global(), "console", consoleMap,
                             std::map<std::string, std::string>());
    mV8Runtime->injectFunction("requestAnimationFrame", requestAnimationFrameCallback, this);
    mV8Runtime->injectFunction("cancelAnimationFrame", cancelAnimationFrameCallback, this);
}

/**
 * let gl = Agil.createContext('webgl');
 * gl.xxx
 */
void AgilV8App::injectAgil() {
    mV8Runtime->injectFunction("registerSurfaceCreate", registerSurfaceCreate, this);
    auto createContext = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto app = static_cast<AgilV8App *>(data->Value());
        assert(app);
        assert(args.Length() == 1 && args[0]->IsString());
        auto result = app->mV8Runtime->utf8(args[0].As<v8::String>());
        if (result == "webgl") {
            auto gl = app->injectWebGL();
            args.GetReturnValue().Set(gl);
        } else {
            ALOGE("only support webgl context")
            args.GetReturnValue().Set(v8::Undefined(app->mV8Runtime->getIsolate()));
        }
    };
    std::map<std::string, v8::FunctionCallback> agilMap(
            {
                    {"createContext", createContext}
            }
    );
    std::map<std::string, std::string> agilValue(
            {
                    {"version", "0.0.1"}
            }
    );
    auto agil = mV8Runtime->injectObject(mV8Runtime->global(), "Agil", agilMap, agilValue, this);
    mAgil.Reset(mV8Runtime->getIsolate(), agil);
}

v8::Local<v8::Object> AgilV8App::injectWebGL() {
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
                    {"deleteProgram",           glDeleteProgramFunc},
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
    return mV8Runtime->injectObject(mV8Runtime->global(), "gl", glMap, constMap);
}

void AgilV8App::injectNodeApi() {
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
    std::map<const char *, v8::FunctionCallback> fileFuncMap(
            {
                    {"exist", [](const v8::FunctionCallbackInfo<v8::Value> &args) {
                        assert(args.Length() == 0);
                        v8::Local<v8::Object> self = args.Holder();
                        v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(
                                self->GetInternalField(0));
                        void *ptr = wrap->Value();
                        args.GetReturnValue().Set(static_cast<File *>(ptr)->exist());
                    }},
                    {"read",  [](const v8::FunctionCallbackInfo<v8::Value> &args) {
                        assert(args.Length() == 0);
                        auto isolate = args.GetIsolate();
                        v8::Local<v8::Object> self = args.Holder();
                        v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(
                                self->GetInternalField(0));
                        void *ptr = wrap->Value();
                        auto value = static_cast<File *>(ptr)->read();
                        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value.c_str()));
                    }},
            }
    );
    mV8Runtime->injectClass("File", fileConstructor, 1, fileFuncMap, this);
}
