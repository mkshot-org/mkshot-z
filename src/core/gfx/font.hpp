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

#include "core/gfx/etc.hpp"
#include "util/util.hpp"

#include <vector>
#include <string>

struct SDL_IOStream;
struct _TTF_Font;
struct Config;

struct SharedFontStatePrivate;

class SharedFontState
{
public:
	SharedFontState(const Config &conf);
	~SharedFontState();

	/* Called from FS during font cache initialization
	 * (when "Fonts/" is scanned for available assets).
	 * 'io' is an opened handle to a possible font file,
	 * 'filename' is the corresponding path */
	void initFontSetCB(SDL_IOStream &io,
	                   const std::string &filename);

	_TTF_Font *getFont(std::string family,
	                   int size);

	bool fontPresent(std::string family) const;

	static _TTF_Font *openBundled(int size);
    void setDefaultFontFamily(const std::string &family);

private:
	SharedFontStatePrivate *p;
};

struct FontPrivate;

class Font
{
public:
	static bool doesExist(const char *name);

	Font(const std::vector<std::string> *names = 0,
	     int size = 0);

	/* Clone constructor */
	Font(const Font &other);

	~Font();

	const Font &operator=(const Font &o);

	int getSize() const;
	void setSize(int value, bool checkIllegal=true);

	DECL_ATTR( Bold,     bool   )
	DECL_ATTR( Italic,   bool   )
	DECL_ATTR( Color,    Color& )
	DECL_ATTR( Shadow,   bool   )
	DECL_ATTR( Outline,  bool   )
	DECL_ATTR( OutColor, Color& )

	DECL_ATTR_STATIC( DefaultSize,     int    )
	DECL_ATTR_STATIC( DefaultBold,     bool   )
	DECL_ATTR_STATIC( DefaultItalic,   bool   )
	DECL_ATTR_STATIC( DefaultColor,    Color& )
	DECL_ATTR_STATIC( DefaultShadow,   bool   )
	DECL_ATTR_STATIC( DefaultOutline,  bool   )
	DECL_ATTR_STATIC( DefaultOutColor, Color& )

	/* There is no point in providing getters for these,
	 * as the bindings will always return the stored native
	 * string/array object anyway. It's impossible to mirror
	 * in the C++ core.
	 * The core object picks the first existing name from the
	 * passed array and stores it internally (same for default). */
	void setName(const std::vector<std::string> &names);
	static void setDefaultName(const std::vector<std::string> &names,
	                           const SharedFontState &sfs);

	static const std::vector<std::string> &getInitialDefaultNames();
    bool isSolid() const;

	/* Assigns heap allocated objects to object properties;
	 * using this in pure C++ will cause memory leaks
	 * (ie. only to be used in GCed language bindings) */
	void initDynAttribs();
	static void initDefaultDynAttribs();

	static void initDefaults(const SharedFontState &sfs);

	/* internal */
	_TTF_Font *getSdlFont();

private:
	FontPrivate *p;
};


