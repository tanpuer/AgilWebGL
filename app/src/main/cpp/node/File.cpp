#include "File.h"
#include "native_log.h"

File::File(std::string path, AgilV8App *app) : path(std::move(path)) {
    this->app = app;
    ALOGD("File:: new File %s", path.c_str())
}

bool File::exist() {
    if (app == nullptr) {
        return false;
    }
    ALOGD("File:: exist")
    return app->mAssetManager->exist(path.c_str());
}

const std::string File::read() {
    if (app == nullptr) {
        return "";
    }
    ALOGD("File:: read %s", path.c_str())
    return app->mAssetManager->readFile(path.c_str());
}
