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


#include "util/serializable.hpp"
#include "binding/util.hpp"
#include "util/exception.hpp"

template<class C>
static VALUE
serializableDump(int, VALUE *, VALUE self)
{
	Serializable *s = getPrivateData<C>(self);

	int dataSize = s->serialSize();

	VALUE data = rb_str_new(0, dataSize);

	GUARD_EXC( s->serialize(RSTRING_PTR(data)); );

	return data;
}

template<class C>
void
serializableBindingInit(VALUE klass)
{
	_rb_define_method(klass, "_dump", serializableDump<C>);
}


