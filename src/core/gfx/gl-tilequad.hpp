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

#include "core/gfx/etc-internal.hpp"

/* Tiled Quads
 *
 * These functions enable drawing a tiled subrectangle
 * of a texture,
 * but no advanced stuff like rotation, scaling etc.
*/

#include "core/gfx/gl-quadarray.hpp"

namespace TileQuads
{
	/* Calculate needed quad counts */
	int oneDimCount(int tileDimension,
	                int destDimension);
	int twoDimCount(int tileW, int tileH,
	                int destW, int destH);

	/* Build tiling quads */
	int buildH(const IntRect &sourceRect,
	           int width, int x, int y,
	           Vertex *verts);

	int buildV(const IntRect &sourceRect,
	           int height, int ox, int oy,
	           Vertex *verts);

	int build(const IntRect &sourceRect,
	          const IntRect &destRect,
	          Vertex *verts);

	/* Build a quad "frame" (see Window cursor_rect) */
	int buildFrame(const IntRect &rect,
	               Vertex vert[36]);

	int buildFrameSource(const IntRect &rect,
	                     Vertex vert[36]);
}


