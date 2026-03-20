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

#pragma once

#include "core/gfx/gl-fun.hpp"

#include <stdio.h>
#include <algorithm>

struct GLDebugLoggerPrivate;

class GLDebugLogger
{
public:
	GLDebugLogger(const char *filename = 0);
	~GLDebugLogger();

private:
	GLDebugLoggerPrivate *p;
};

#define GL_MARKER(format, ...) \
	if (gl.StringMarker) \
	{ \
		char buf[128]; \
		int len = snprintf(buf, sizeof(buf), format, ##__VA_ARGS__); \
		gl.StringMarker(std::min<size_t>(len, sizeof(buf)), buf); \
	}



