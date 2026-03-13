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


#include "util/disposable.hpp"
#include "binding/util.hpp"
#include "core/gfx/gfx.hpp"

template<class C>
RB_METHOD(disposableDispose)
{
	RB_UNUSED_PARAM;

	C *d = getPrivateData<C>(self);

	if (!d)
		return Qnil;

	/* Nothing to do if already disposed */
	if (d->isDisposed())
		return Qnil;

    GFX_LOCK;
	d->dispose();
    GFX_UNLOCK;

	return Qnil;
}

template<class C>
RB_METHOD(disposableIsDisposed)
{
	RB_UNUSED_PARAM;

	C *d = getPrivateData<C>(self);

	if (!d)
		return Qtrue;

	return rb_bool_new(d->isDisposed());
}

template<class C>
static void disposableBindingInit(VALUE klass)
{
	_rb_define_method(klass, "dispose", disposableDispose<C>);
	_rb_define_method(klass, "disposed?", disposableIsDisposed<C>);
}

template<class C>
inline void
checkDisposed(VALUE self)
{
	if (disposableIsDisposed<C>(0, 0, self) == Qtrue)
		raiseDisposedAccess(self);
}


