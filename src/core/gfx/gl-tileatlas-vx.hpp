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

#include <stdlib.h>

struct FloatRect;
struct TEXFBO;
class Bitmap;
class Table;

#define ATLASVX_W 1024
 2048

/* Bitmap indices */
enum
{
	BM_A1 = 0,
	BM_A2 = 1,
	BM_A3 = 2,
	BM_A4 = 3,
	BM_A5 = 4,
	BM_B  = 5,
	BM_C  = 6,
	BM_D  = 7,
	BM_E  = 8,

	BM_COUNT
};

namespace TileAtlasVX
{
struct Reader
{
	virtual void onQuads(const FloatRect *t, const FloatRect *p,
	                     size_t n, bool overPlayer) = 0;
};

void build(TEXFBO &tf, Bitmap *bitmaps[BM_COUNT]);

void readTiles(Reader &reader, const Table &data,
               const Table *flags, int ox, int oy, int w, int h);
}


