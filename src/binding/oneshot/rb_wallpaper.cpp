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

#include "binding/rb_util.hpp"
#include "core/shared-state.hpp"
#include "core/oneshot/oneshot.hpp"
#include "core/oneshot/wallpaper.hpp"

RB_METHOD(wallpaperSet)
{
	RB_UNUSED_PARAM;

	const char *name;
	int color;
	rb_get_args(argc, argv, "zi", &name, &color RB_ARG_END);

	shState->oneshot().wallpaper->set(name, color);

	return Qnil;
}

RB_METHOD(wallpaperReset)
{
	RB_UNUSED_PARAM;

	shState->oneshot().wallpaper->reset();

	return Qnil;
}

void wallpaperBindingInit()
{
	VALUE module = rb_define_module("Wallpaper");

	_rb_define_module_function(module, "set", wallpaperSet);
	_rb_define_module_function(module, "reset", wallpaperReset);
}
