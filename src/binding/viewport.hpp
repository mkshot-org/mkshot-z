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


#include "core/gfx/viewport.hpp"
#include "core/shared-state.hpp"
#include "binding/util.hpp"
#include "binding/types.hpp"
#include "core/gfx/gfx.hpp"

#include "binding/scene.hpp"
#include "binding/disposable.hpp"

template<class C>
RB_METHOD(viewportElementGetViewport)
{
	RB_UNUSED_PARAM;

	checkDisposed<C>(self);

	return rb_iv_get(self, "viewport");
}

template<class C>
RB_METHOD(viewportElementSetViewport)
{
	RB_UNUSED_PARAM;

	ViewportElement *ve = getPrivateData<C>(self);

	VALUE viewportObj = Qnil;
	Viewport *viewport = 0;

	rb_get_args(argc, argv, "o", &viewportObj RB_ARG_END);

	if (!NIL_P(viewportObj))
		viewport = getPrivateDataCheck<Viewport>(viewportObj, ViewportType);

	GFX_GUARD_EXC( ve->setViewport(viewport); );

	rb_iv_set(self, "viewport", viewportObj);

	return viewportObj;
}

template<class C>
static C *
viewportElementInitialize(int argc, VALUE *argv, VALUE self)
{
	/* Get parameters */
	VALUE viewportObj = Qnil;
	Viewport *viewport = 0;

	rb_get_args(argc, argv, "|o", &viewportObj RB_ARG_END);

	if (!NIL_P(viewportObj))
	{
		viewport = getPrivateDataCheck<Viewport>(viewportObj, ViewportType);
	}

    GFX_LOCK;
	/* Construct object */
	C *ve = new C(viewport);

    
	/* Set property objects */
	rb_iv_set(self, "viewport", viewportObj);
    GFX_UNLOCK;
	return ve;
}

template<class C>
void
viewportElementBindingInit(VALUE klass)
{
	sceneElementBindingInit<C>(klass);

	_rb_define_method(klass, "viewport", viewportElementGetViewport<C>);

    //if (rgssVer >= 2)
	//{
	_rb_define_method(klass, "viewport=", viewportElementSetViewport<C>);
	//}
}


