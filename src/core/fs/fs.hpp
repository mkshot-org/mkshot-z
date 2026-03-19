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

#pragma once

#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_video.h>
#include <string>

namespace mkshot_fs {
	bool fileExists(const char *path);

	std::string contentsOfFileAsString(const char *path);

	bool setCurrentDirectory(const char *path);

	std::string getCurrentDirectory();
	std::string normalizePath(const char *path, bool preferred, bool absolute);
	std::string getDefaultGameRoot();
#ifdef __APPLE__
	std::string getPathForAsset(const char *baseName, const char *ext);
	std::string contentsOfAssetAsString(const char *baseName, const char *ext);
	std::string getResourcePath();
	std::string selectPath(SDL_Window *win, const char *msg, const char *prompt);
#endif
};

struct FSPrivate;
class SharedFontState;

class FS
{
public:
	FS(const char *argv0,
	           bool allowSymlinks);
	~FS();

	void addPath(const char *path, const char *mountpoint = 0, bool reload = false);
    void removePath(const char *path, bool reload = false);

	/* Call these after the last 'addPath()' */
	void createPathCache();
    void reloadPathCache();

	/* Scans "Fonts/" and creates inventory of
	 * available font assets */
	void initFontSets(SharedFontState &sfs);

	struct OpenHandler
	{
		/* Try to read and interpret data provided from io.
		 * If data cannot be parsed, return false, otherwise true.
		 * Can be called multiple times until a parseable file is found.
		 * It's the handler's responsibility to close every passed
		 * io structure, even when data could not be parsed.
		 * After this function returns, io becomes invalid, so don't take
		 * references to it. Instead, copy the structure without closing
		 * if you need to further read from it later. */
		virtual bool tryRead(SDL_IOStream &io, const char *ext) = 0;
	};

	void openRead(OpenHandler &handler,
	              const char *filename);

	/* Circumvents extension supplementing */
	void openReadRaw(SDL_IOStream &io,
	                 const char *filename,
	                 bool freeOnClose = false);

	std::string normalize(const char *pathname, bool preferred, bool absolute);

	/* Does not perform extension supplementing */
	bool exists(const char *filename);

	const char *desensitize(const char *filename);

private:
	FSPrivate *p;
};

extern const Uint32 SDL_IOStream_PHYSFS;
