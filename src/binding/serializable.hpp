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


