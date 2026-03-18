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
#include "util/util.hpp"

struct ViewportPrivate;

class Viewport : public Scene, public SceneElement, public Flashable, public Disposable
{
public:
	Viewport(int x, int y, int width, int height);
	Viewport(Rect *rect);
	Viewport();
	~Viewport();

	void update();

	DECL_ATTR( Rect,  Rect&  )
	DECL_ATTR( OX,    int    )
	DECL_ATTR( OY,    int    )
	DECL_ATTR( Color, Color& )
	DECL_ATTR( Tone,  Tone&  )

	void initDynAttribs();

private:
	void initViewport(int x, int y, int width, int height);
	void geometryChanged();

	void composite();
	void draw();
	void onGeometryChange(const Geometry &);
	bool isEffectiveViewport(Rect *&, Color *&, Tone *&) const;

	void releaseResources();
	const char *klassName() const { return "viewport"; }

	ABOUT_TO_ACCESS_DISP

	ViewportPrivate *p;
	friend struct ViewportPrivate;

	IntruListLink<Scene> sceneLink;
};

class ViewportElement : public SceneElement
{
public:
	ViewportElement(Viewport *viewport = 0, int z = 0, int spriteY = 0);
	~ViewportElement();

	DECL_ATTR( Viewport,  Viewport* )

protected:
	virtual void onViewportChange() {}

private:
	Viewport *m_viewport;
	sigslot::connection viewportDispCon;
	sigslot::connection viewportElementDispCon;
	void viewportElementDisposal();
};


