//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLCOMPILESHADERFUNC_H
#define AGILV8_GLCOMPILESHADERFUNC_H

#include "v8.h"

auto glCompileShaderFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glCompileShader Invalid argument"));
        return;
    }
    auto shader = args[0].As<v8::Number>()->Int32Value();;
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;
        // 查询日志的长度判断是否有日志产生
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            // 分配一个足以存储日志信息的字符串
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            // 检索日志信息
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            ALOGE("Error compiling shader:\n%s\n", infoLog);
            // 使用完成后需要释放字符串分配的内存
            free(infoLog);
        }
        // 删除编译出错的着色器释放内存
        glDeleteShader(shader);
    }
};

#endif //AGILV8_GLCOMPILESHADERFUNC_H
