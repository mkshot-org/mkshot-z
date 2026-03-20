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

#include <SDL3/SDL_atomic.h>
#include <SDL3/SDL_thread.h>
#include <SDL3/SDL_iostream.h>

#include <string>
#include <iostream>
#include <unistd.h>

struct AtomicFlag
{
	AtomicFlag()
	{
		clear();
	}

	AtomicFlag(bool value)
	{
		SDL_SetAtomicInt(&atom, value ? 1 : 0);
	}

	void set()
	{
		SDL_SetAtomicInt(&atom, 1);
	}

	void clear()
	{
		SDL_SetAtomicInt(&atom, 0);
	}

    void wait()
    {
        while (SDL_GetAtomicInt(&atom)) {}
    }

    void reset()
    {
        wait();
        set();
    }

	operator bool() const
	{
		return SDL_GetAtomicInt(&atom);
	}

private:
	mutable SDL_AtomicInt atom;
};

template<class C, void (C::*func)()>
int __sdlThreadFun(void *obj)
{
	(static_cast<C*>(obj)->*func)();
	return 0;
}

template<class C, void (C::*func)()>
SDL_Thread *createSDLThread(C *obj, const std::string &name = std::string())
{
	return SDL_CreateThread((__sdlThreadFun<C, func>), name.c_str(), obj);
}

inline bool readFileSDL(const char *path,
                        std::string &out)
{
	SDL_IOStream *f = SDL_IOFromFile(path, "rb");

	if (!f)
		return false;

	long size = SDL_GetIOSize(f);
	size_t back = out.size();

	out.resize(back+size);
	size_t read = SDL_ReadIO(f, &out[back], size);
	SDL_CloseIO(f);

	if (read != (size_t) size)
		out.resize(back+read);

	return true;
}

template<size_t bufSize = 248, size_t pbSize = 8>
class SDLRWBuf : public std::streambuf
{
public:
	SDLRWBuf(SDL_IOStream *io)
	    : io(io)
	{
		char *end = buf + bufSize + pbSize;
		setg(end, end, end);
	}

private:
	int_type underflow()
	{
		if (!io)
			return traits_type::eof();

		if (gptr() < egptr())
			return traits_type::to_int_type(*gptr());

		char *base = buf;
		char *start = base;

		if (eback() == base)
		{
			memmove(base, egptr() - pbSize, pbSize);
			start += pbSize;
		}

		size_t n = SDL_ReadIO(io, start, bufSize - (start - base));
		if (n == 0)
			return traits_type::eof();

		setg(base, start, start + n);

		return underflow();
	}

	SDL_IOStream *io;
	char buf[bufSize+pbSize];
};

class SDLRWStream
{
public:
	SDLRWStream(const char *filename,
	            const char *mode)
	    : io(SDL_IOFromFile(filename, mode)),
	      buf(io),
	      s(&buf)
	{}

	~SDLRWStream()
	{
		if (io)
			SDL_CloseIO(io);
	}

	operator bool() const
	{
		return io != 0;
	}

	std::istream &stream()
	{
		return s;
	}

private:
	SDL_IOStream *io;
	SDLRWBuf<> buf;
	std::istream s;
};


