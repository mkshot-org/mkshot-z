/*
** This file is part of mkxp-z, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Created by ゾロアーク on 11/21/20.
**
** mkxp-z is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#pragma once


#include <string>
#include <SDL_video.h>

namespace filesystemImpl {
bool fileExists(const char *path);

std::string contentsOfFileAsString(const char *path);

bool setCurrentDirectory(const char *path);
    
std::string getCurrentDirectory();
    
std::string normalizePath(const char *path, bool preferred, bool absolute);

std::string getDefaultGameRoot();

#ifdef MKSHOT_BUILD_XCODE
std::string getPathForAsset(const char *baseName, const char *ext);
std::string contentsOfAssetAsString(const char *baseName, const char *ext);

std::string getResourcePath();

std::string selectPath(SDL_Window *win, const char *msg, const char *prompt);
#endif

};
