/*
** mkshot-z - Experimental OneShot (2016) engine reimplementation for modders.
**
** Copyright (C) 2026 Team Reverium <https://github.com/reverium>
** Copyright (C) 2024 hat_kid <https://github.com/thehatkid> (ModShot-mkxp-z)
** Copyright (C) 2013-2023 Amaryllis Kulla and mkxp-z contributors
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
*/

#include <SDL3/SDL_filesystem.h>

#include "core/fs/fs-impl.hpp"
#include "util/exception.hpp"
#include "util/dbg-writer.hpp"

namespace fs = std::filesystem;

#include <fstream>

// https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool filesystemImpl::fileExists(const char *path) {
    fs::path stdPath(path);
    return (fs::exists(stdPath) && !fs::is_directory(stdPath));
}


// https://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c
std::string filesystemImpl::contentsOfFileAsString(const char *path) {
    std::string ret;
    try {
        std::ifstream ifs(path);
        ret = std::string ( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()    ) );
    } catch (...) {
        throw Exception(Exception::NoFileError, "Failed to read file at %s", path);
    }

    return ret;
}

// chdir and getcwd do not support unicode on Windows
bool filesystemImpl::setCurrentDirectory(const char *path) {
    fs::path stdPath(path);
    fs::current_path(stdPath);
    bool ret;

    try {
        ret = fs::equivalent(fs::current_path(), stdPath);
    } catch (...) {
        Debug() << "Failed to check current path." << path;
        ret = false;
    }
    return ret;
}

std::string filesystemImpl::getCurrentDirectory() {
    std::string ret;
    try {
        ret = std::string(fs::current_path().string());
    } catch (...) {
        throw Exception(Exception::MKShotError, "Failed to retrieve current path");
    }
    return ret;
}


std::string filesystemImpl::normalizePath(const char *path, bool preferred, bool absolute) {
    fs::path stdPath(path);
    
    if (!stdPath.is_absolute() && absolute)
        stdPath = fs::current_path() / stdPath;

    stdPath = stdPath.lexically_normal();
    std::string ret(stdPath);
    for (size_t i = 0; i < ret.length(); i++) {
        char sep;
        char sep_alt;
#ifdef __WIN32__
        if (preferred) {
            sep = '\\';
            sep_alt = '/';
        }
        else
#endif
        {
            sep = '/';
            sep_alt = '\\';
        }
        
        if (ret[i] == sep_alt)
            ret[i] = sep;
    }
    return ret;
}

std::string filesystemImpl::getDefaultGameRoot() {
    char *p = SDL_GetBasePath();
    std::string ret(p);
    SDL_free(p);
    return ret;
}
