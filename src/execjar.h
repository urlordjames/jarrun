#include <filesystem>
#include <string.h>
#ifndef _WIN32
#include <jni.h>
#endif

void runJar(std::filesystem::path path, std::string classpath);
