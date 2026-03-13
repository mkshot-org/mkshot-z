/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#pragma once


#include "core/gfx/scene.hpp"
#include "binding/util.hpp"
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


