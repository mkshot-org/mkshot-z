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

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static inline int
wrapRange(int value, int min, int max)
{
	if (value >= min && value <= max)
		return value;

	while (value < min)
		value += (max - min);

	return value % (max - min);
}

static inline int
findNextPow2(int start)
{
	int i = 1;
	while (i < start)
		i <<= 1;

	return i;
}

/* Reads the contents of the file at 'path' and
** appends them to 'out'. Returns false on failure
*/
inline bool readFile(const char *path,
                     std::string &out)
{
    size_t size;
    void *data = SDL_LoadFile(path, &size);

    if (!data) {
        SDL_Log("Failed to read %s: %s", path, SDL_GetError());
        return false;
    }

    out.append(static_cast<const char*>(data), size);

    SDL_free(data);
    return true;
}

/* Check if [C]ontainer contains [V]alue */
template<typename C, typename V>
inline bool contains(const C &c, const V &v)
{
	return std::find(c.begin(), c.end(), v) != c.end();
}

template<typename C>
inline const C *dataPtr(const std::vector<C> &v)
{
	return v.empty() ? (C*)0 : &v[0];
}

template<typename C>
inline C *dataPtr(std::vector<C> &v)
{
	return v.empty() ? (C*)0 : &v[0];
}

#define ARRAY_SIZE(obj) (sizeof(obj) / sizeof((obj)[0]))

#define elementsN(obj) const size_t obj##N = ARRAY_SIZE(obj)

#define DECL_ATTR_DETAILED(name, type, keyword1, keyword2) \
	keyword1 type get##name() keyword2; \
	keyword1 void set##name(type value);

#define DECL_ATTR(name, type) DECL_ATTR_DETAILED(name, type, , const)
#define DECL_ATTR_VIRT(name, type) DECL_ATTR_DETAILED(name, type, virtual, const)
#define DECL_ATTR_STATIC(name, type) DECL_ATTR_DETAILED(name, type, static, )
#define DECL_ATTR_INLINE(name, type, loc) \
	type get##name() const { return loc; } \
	void set##name(type value) { loc = value; }

#define DEF_ATTR_RD_SIMPLE_DETAILED(klass, name, type, location, keyword1) \
	type klass :: get##name() keyword1 \
	{ \
		guardDisposed(); \
		return location; \
	}

#define DEF_ATTR_SIMPLE_DETAILED(klass, name, type, location, keyword1) \
	DEF_ATTR_RD_SIMPLE_DETAILED(klass, name, type, location, keyword1) \
	void klass :: set##name(type value) \
{ \
	guardDisposed(); \
	location = value; \
}

#define DEF_ATTR_RD_SIMPLE(klass, name, type, location) \
	DEF_ATTR_RD_SIMPLE_DETAILED(klass, name, type, location, const)
#define DEF_ATTR_SIMPLE(klass, name, type, location) \
	DEF_ATTR_SIMPLE_DETAILED(klass, name, type, location, const)

#define DEF_ATTR_SIMPLE_STATIC(klass, name, type, location) \
	DEF_ATTR_SIMPLE_DETAILED(klass, name, type, location, )
