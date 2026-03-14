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


