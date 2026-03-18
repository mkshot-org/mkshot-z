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

#include "core/gfx/scene.hpp"
#include "core/gfx/flashable.hpp"
#include "util/disposable.hpp"
#include "core/gfx/viewport.hpp"
#include "util/util.hpp"

class Bitmap;
struct Color;
struct Tone;
struct Rect;

struct SpritePrivate;

class Sprite : public ViewportElement, public Flashable, public Disposable
{
public:
	Sprite(Viewport *viewport = 0);
	~Sprite();

	int getWidth()  const;
	int getHeight() const;

	void update();

	DECL_ATTR( Bitmap,      Bitmap* )
	DECL_ATTR( SrcRect,     Rect&   )
	DECL_ATTR( X,           int     )
	DECL_ATTR( Y,           int     )
	DECL_ATTR( OX,          int     )
	DECL_ATTR( OY,          int     )
	DECL_ATTR( ZoomX,       float   )
	DECL_ATTR( ZoomY,       float   )
	DECL_ATTR( Angle,       float   )
	DECL_ATTR( Mirror,      bool    )
	DECL_ATTR( BushDepth,   int     )
	DECL_ATTR( BushOpacity, int     )
	DECL_ATTR( Opacity,     int     )
	DECL_ATTR( BlendType,   int     )
	DECL_ATTR( Color,       Color&  )
	DECL_ATTR( Tone,        Tone&   )
    DECL_ATTR( Pattern,     Bitmap* )
    DECL_ATTR( PatternBlendType, int)
    DECL_ATTR( PatternTile, bool    )
    DECL_ATTR( PatternOpacity, int  )
    DECL_ATTR( PatternScrollX, int  )
    DECL_ATTR( PatternScrollY, int  )
    DECL_ATTR( PatternZoomX, float  )
    DECL_ATTR( PatternZoomY, float  )
    DECL_ATTR( Invert,      bool    )
	DECL_ATTR( Obscured,    bool    )
	DECL_ATTR( WaveAmp,     int     )
	DECL_ATTR( WaveLength,  int     )
	DECL_ATTR( WaveSpeed,   int     )
	DECL_ATTR( WavePhase,   float   )

	void initDynAttribs();

private:
	SpritePrivate *p;

	void draw();
	void onGeometryChange(const Scene::Geometry &);

	void releaseResources();
	const char *klassName() const { return "sprite"; }

	ABOUT_TO_ACCESS_DISP
};


