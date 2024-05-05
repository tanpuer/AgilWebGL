#pragma once

#include "string"
#include "memory"
#include "AgilV8App.h"

class File {

public:

    File() = delete;

    File(File &file) = delete;

    void operator=(File &file) = delete;

    File(std::string path, AgilV8App* assetManager);

    bool exist();

    const std::string read();

private:

    std::string path;

    AgilV8App* app = nullptr;

};
