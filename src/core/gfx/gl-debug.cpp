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

#include "core/gfx/gl-debug.hpp"
#include "util/dbg-writer.hpp"

#include <iostream>

#include "core/gfx/gl-fun.hpp"

struct GLDebugLoggerPrivate
{
	std::ostream *stream;

	GLDebugLoggerPrivate(const char *logFilename)
	{
		(void) logFilename;

		stream = &std::clog;
	}

	~GLDebugLoggerPrivate()
	{
	}

	void writeTimestamp()
	{
		// FIXME reintroduce proper time stamps (is this even necessary??)
		*stream << "[GLDEBUG] ";
	}

	void writeLine(const char *line)
	{
		*stream << line << "\n";
		stream->flush();
	}
};

static void APIENTRY arbDebugFunc(GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar* message,
                                  const void* userParam)
{
	GLDebugLoggerPrivate *p =
		static_cast<GLDebugLoggerPrivate*>(const_cast<void*>(userParam));

	(void) source;
	(void) type;
	(void) id;
	(void) severity;
	(void) length;

	p->writeTimestamp();
	p->writeLine(message);
}

GLDebugLogger::GLDebugLogger(const char *filename)
{
	p = new GLDebugLoggerPrivate(filename);

	if (gl.DebugMessageCallback)
		gl.DebugMessageCallback(arbDebugFunc, p);
	else
		Debug() << "DebugLogger: no debug extensions found";
}

GLDebugLogger::~GLDebugLogger()
{
	delete p;
}
