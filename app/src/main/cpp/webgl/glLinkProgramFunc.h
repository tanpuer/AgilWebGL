//
// Created by banma-3412 on 2023/12/5.
//

#ifndef AGILV8_GLLINKPROGRAMFUNC_H
#define AGILV8_GLLINKPROGRAMFUNC_H

#include "v8.h"

auto glLinkProgramFunc = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "glLinkProgram Invalid argument"));
        return;
    }
    auto program = v8::Local<v8::Number>::Cast(args[0])->Int32Value();
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        // 检查日志信息长度
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            // 分配一个足以存储日志信息的字符串
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            // 检索日志信息
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            ALOGE("Error linking program:\n%s\n", infoLog);
            // 使用完成后需要释放字符串分配的内存
            free(infoLog);
        }
        // 删除着色器释放内存
        glDeleteProgram(program);
    }
};

#endif //AGILV8_GLLINKPROGRAMFUNC_H
