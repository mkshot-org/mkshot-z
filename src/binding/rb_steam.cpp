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

#ifdef MKSHOT_STEAM
#include "core/shared-state.hpp"
#include "core/steam/steam.hpp"
#endif

RB_METHOD(steamEnabled)
{
	RB_UNUSED_PARAM;

#ifdef MKSHOT_STEAM
	return Qtrue;
#else
	return Qfalse;
#endif
}

RB_METHOD(steamUnlock)
{
	RB_UNUSED_PARAM;

	const char *name;
	rb_get_args(argc, argv, "z", &name RB_ARG_END);

#ifdef MKSHOT_STEAM
	shState->steam().unlock(name);
#endif

	return Qnil;
}

RB_METHOD(steamLock)
{
	RB_UNUSED_PARAM;

	const char *name;
	rb_get_args(argc, argv, "z", &name RB_ARG_END);

#ifdef MKSHOT_STEAM
	shState->steam().lock(name);
#endif

	return Qnil;
}

RB_METHOD(steamUnlocked)
{
	RB_UNUSED_PARAM;

	const char *name;
	rb_get_args(argc, argv, "z", &name RB_ARG_END);

#ifdef MKSHOT_STEAM
	return shState->steam().isUnlocked(name) ? Qtrue : Qfalse;
#else
	return Qfalse;
#endif
}

void steamBindingInit()
{
	VALUE module = rb_define_module("Steam");

#ifdef MKSHOT_STEAM
	rb_const_set(module, rb_intern("USER_NAME"), rb_str_new2(shState->steam().userName().c_str()));
	if (shState->steam().lang().empty())
		rb_const_set(module, rb_intern("LANG"), Qnil);
	else
		rb_const_set(module, rb_intern("LANG"), rb_str_new2(shState->steam().lang().c_str()));
#else
	rb_const_set(module, rb_intern("USER_NAME"), Qnil);
	rb_const_set(module, rb_intern("LANG"), Qnil);
#endif

	_rb_define_module_function(module, "enabled?", steamEnabled);
	_rb_define_module_function(module, "unlock", steamUnlock);
	_rb_define_module_function(module, "lock", steamLock);
	_rb_define_module_function(module, "unlocked?", steamUnlocked);
}
