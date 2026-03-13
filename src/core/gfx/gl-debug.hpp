/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#pragma once


#include "core/gfx/fun.hpp"

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



