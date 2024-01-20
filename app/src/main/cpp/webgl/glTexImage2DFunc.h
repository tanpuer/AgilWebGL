//
// Created by banma-3412 on 2023/12/6.
//

#ifndef AGILV8_GLTEXIMAGE2DFUNC_H
#define AGILV8_GLTEXIMAGE2DFUNC_H

#include "v8.h"
#include "base/gl_utils.h"
#include "stb_image.h"
#include "AgilV8App.h"

auto glTexImage2DFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 6) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glTexImage2D Invalid argument"));
        return;
    }
    auto target = args[0].As<v8::Number>()->Int32Value();
    auto level = args[1].As<v8::Number>()->Int32Value();
    auto internalFormat = args[2].As<v8::Number>()->Int32Value();
    auto format = args[3].As<v8::Number>()->Int32Value();
    auto type = args[4].As<v8::Number>()->Int32Value();
    if (args[5]->IsString()) {
        v8::String::Utf8Value value(isolate, args[5]);
        auto imagePathStr = std::string(*value, value.length());
        const char *str = imagePathStr.c_str();

        //todo 异步加载图片
        int x, y, comp;
        auto fileloc = str;
        auto app = static_cast<AgilV8App *>(v8::Local<v8::External>::Cast(args.Data())->Value());
        assert(app);
        auto assetManager = app->mAssetManager;
        if (assetManager == nullptr) {
            isolate->ThrowException(
                    v8::String::NewFromUtf8(isolate, "glTexImage2D unsupported argument"));
            return;
        }
        auto imageData = assetManager->readImage(fileloc);
        unsigned char *data = stbi_load_from_memory(imageData->content, imageData->length, &x, &y,
                                                    &comp, STBI_default);
        delete imageData;
        GLuint imageFormat = GL_RGB;
        if (comp == 1) {
            imageFormat = GL_LUMINANCE;
        } else if (comp == 2) {
            imageFormat = GL_LUMINANCE_ALPHA;
        } else if (comp == 3) {
            imageFormat = GL_RGB;
        } else if (comp == 4) {
            imageFormat = GL_RGBA;
        } else {
            ALOGE("unSupport type %d %s", comp, fileloc);
        }
        if (nullptr != data) {
            glTexImage2D(target, level, imageFormat, x, y, 0, imageFormat, type, data);
            stbi_image_free(data);
            ALOGD("load image success %s", fileloc)
        } else {
            ALOGE("load image fail %s", fileloc)
        }
    } else if (args[5]->IsUint32Array()) {
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glTexImage2D unsupported argument"));
//        auto uint32Array = args[8].As<v8::Uint32Array>();
//        v8::Local<v8::ArrayBuffer> buffer = uint32Array->Buffer();
//        v8::ArrayBuffer::Contents contents = buffer->GetContents();
//        void *data = contents.Data();
//        size_t offset = uint32Array->ByteOffset();
//        auto pixels = static_cast<uint32_t *>(data) + offset / sizeof(float);
//        glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);
    } else {
        //todo 还可以传其他格式
        isolate->ThrowException(
                v8::String::NewFromUtf8(isolate, "glTexImage2D unsupported argument"));
    }
    checkGLError("glTexImage2D");
};

#endif //AGILV8_GLTEXIMAGE2DFUNC_H
