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

#include "core/gfx/gl-scene.hpp"
#include "binding/rb_util.hpp"
#include "core/gfx/gfx.hpp"

template<class C>
RB_METHOD(sceneElementGetZ)
{
	RB_UNUSED_PARAM;

	SceneElement *se = getPrivateData<C>(self);

	int value = 0;
	GUARD_EXC( value = se->getZ(); );

	return rb_fix_new(value);
}

template<class C>
RB_METHOD(sceneElementSetZ)
{
	SceneElement *se = getPrivateData<C>(self);

	int z;
	rb_get_args(argc, argv, "i", &z RB_ARG_END);

	GFX_GUARD_EXC( se->setZ(z); );

	return rb_fix_new(z);
}

template<class C>
RB_METHOD(sceneElementGetVisible)
{
	RB_UNUSED_PARAM;

	SceneElement *se = getPrivateData<C>(self);

	bool value = false;
	GUARD_EXC( value = se->getVisible(); );

	return rb_bool_new(value);
}

template<class C>
RB_METHOD(sceneElementSetVisible)
{
	SceneElement *se = getPrivateData<C>(self);

	bool visible;
	rb_get_args(argc, argv, "b", &visible RB_ARG_END);

	GFX_GUARD_EXC( se->setVisible(visible); );

    return rb_bool_new(visible);
}

template<class C>
void
sceneElementBindingInit(VALUE klass)
{
	_rb_define_method(klass, "z",        sceneElementGetZ<C>);
	_rb_define_method(klass, "z=",       sceneElementSetZ<C>);
	_rb_define_method(klass, "visible",  sceneElementGetVisible<C>);
	_rb_define_method(klass, "visible=", sceneElementSetVisible<C>);
}


