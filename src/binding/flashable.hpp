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


