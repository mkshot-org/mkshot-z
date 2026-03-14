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


#include "core/gfx/flashable.hpp"
#include "binding/util.hpp"
#include "binding/types.hpp"

template<class C>
RB_METHOD(flashableFlash)
{
	Flashable *f = getPrivateData<C>(self);

	VALUE colorObj;
	int duration;

	Color *color;

	rb_get_args(argc, argv, "oi", &colorObj, &duration RB_ARG_END);

	if (NIL_P(colorObj))
	{
		f->flash(0, duration);
		return Qnil;
	}

	color = getPrivateDataCheck<Color>(colorObj, ColorType);

	f->flash(&color->norm, duration);

	return Qnil;
}

template<class C>
RB_METHOD(flashableUpdate)
{
	RB_UNUSED_PARAM;

	Flashable *f = getPrivateData<C>(self);

	f->update();

	return Qnil;
}

template<class C>
static void flashableBindingInit(VALUE klass)
{
	_rb_define_method(klass, "flash", flashableFlash<C>);
	_rb_define_method(klass, "update", flashableUpdate<C>);
}


