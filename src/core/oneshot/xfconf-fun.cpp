/*
** mkshot-z - Experimental OneShot (2016) engine reimplementation for modders.
**
** Copyright (C) 2026 Reverium <https://github.com/reverium>
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

#include "core/oneshot/xfconf-fun.hpp"
#include "util/dbg-writer.hpp"

#include <vector>

#include <SDL3/SDL_loadso.h>

#ifdef __linux__
#define XFCONF_LIBNAME "libxfconf-0.so"
#else
#error "This code is currently only for Linux platforms."
#endif

struct XfconfFunctions dynXfconf;

#define XFCONF_FUNC(name, type) \
	dynXfconf.name = (type)SDL_LoadFunction(so, "xfconf_" #name); \
	if (dynXfconf.name == NULL) { \
		Debug() << "[xfconf-fun] Unable to load function:" << SDL_GetError(); \
		fail = true; \
	}

void initXfconfFunctions()
{
	bool fail = false;

	static SDL_SharedObject *so;

	std::vector<const char *> sonames {
		XFCONF_LIBNAME ".3",
		XFCONF_LIBNAME ".2",
		XFCONF_LIBNAME
	};

	for (const char *name : sonames) {
		so = SDL_LoadObject(name);

		if (so != NULL)
			break;
	}

	if (so == NULL) {
		Debug() << "[xfconf-fun] Couldn't load library:" << SDL_GetError();
		fail = true;
	}

	if (!fail) {
		XFCONF_FUNCS
	}

	if (fail) {
		memset(&dynGio, 0, sizeof(dynGio));
		SDL_UnloadObject(so);
		so = nullptr;
	}
}
