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


#include "util/disposable.hpp"
#include "util/util.hpp"

class Viewport;
class Bitmap;
class Table;

struct TilemapVXPrivate;

class TilemapVX : public Disposable
{
public:
	class BitmapArray
	{
	public:
		void set(int i, Bitmap *bitmap);
		Bitmap *get(int i) const;

	private:
		BitmapArray() {}
		~BitmapArray() {}

		TilemapVXPrivate *p;
		friend class TilemapVX;
		friend struct TilemapVXPrivate;
	};

	TilemapVX(Viewport *viewport = 0);
	~TilemapVX();

	void update();

	BitmapArray &getBitmapArray();

	DECL_ATTR( Viewport,   Viewport* )
	DECL_ATTR( MapData,    Table*    )
	DECL_ATTR( FlashData,  Table*    )
	DECL_ATTR( Flags,      Table*    )
	DECL_ATTR( Visible,    bool      )
	DECL_ATTR( OX,         int       )
	DECL_ATTR( OY,         int       )

private:
	TilemapVXPrivate *p;
	BitmapArray bmProxy;

	void releaseResources();
	const char *klassName() const { return "tilemap"; }
};


